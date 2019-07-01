#include <ilcplex/ilocplex.h>
#include "ArgPack.h"
#include "AlphaKruskalTCP.h"
#include "LoadInstance.h"
#include "LNS_TCP.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <experimental/filesystem>
#include <filesystem>
#include <stack>

TEdges LoadEdges(TEdges & InstData);
std::unordered_set<int> LoadTerminals(TEdges & InstData);
FInstance GetInstance(std::string const & InstanceDir);
std::deque<std::string> ListDirectoryFiles(std::string Directory);
void RunInstance(std::string & InstanceDir, int L, int R, int primal_bound, long memory, long time);

using namespace std;

typedef IloArray<IloIntVarArray> Edges;


int find(vector<int>& parent, int i) 
{ 
    // find root and make root as parent of i (path compression) 
    if (parent[i] != i) 
        parent[i] = find(parent, parent[i]); 
  
    return parent[i]; 
} 
  
// A function that does union of two sets of x and y 
// (uses union by rank) 
void Union(vector<int>& parent, vector<int>& rank, int x, int y) 
{ 
    int xroot = find(parent, x); 
    int yroot = find(parent, y); 
  
    // Attach smaller rank tree under root of high rank tree 
    // (Union by Rank) 
    if (rank[xroot] < rank[yroot]) 
        parent[xroot] = yroot; 
    else if (rank[xroot] > rank[yroot]) 
        parent[yroot] = xroot; 
  
    // If ranks are same, then make one as root and increment 
    // its rank by one 
    else
    { 
        parent[yroot] = xroot; 
        rank[xroot]++; 
    } 
} 

inline bool has_edge(IloNumArray2 sol, int i, int j,  IloNum tol)
{

    if( (j < i and sol[j][i-j-1] >= 1.0-tol) or
            (i < j and sol[i][j-i-1] >= 1.0-tol) )
        return true;
    return false;
}

void print_sol(IloNumArray2 sol, IloNum tol, unordered_set<int>& terminals)
{
    cerr << "Solution \n";
    int c=0;
    int n = sol.getSize();
    vector<int> degree(n,0);
    for(int i=0;i<n;++i) 
        for(int j=0;j<sol[i].getSize();++j) 
            if(sol[i][j] >= 1.0-tol) {
                if(i) degree[i]++;
                if(i) degree[j+i+1]++;
                ++c;
                cerr << i << "-" << j+i+1 << " ";
            }
    cerr << endl << "Total: "  << c << endl;

    for(auto& t : terminals)
        degree[t] = 0;

    cout << "Steiner vertex\n";
    int l=0;
    int r=0;
    for(int i=0; i<n;++i)
        if(degree[i]) {
            l++;
            if(degree[i] >= 3) r++;
            cout << i << " degree -> " << degree[i] << endl;
        }

    cout << "|L| = " << l-r << " |R| " << r << endl;
}

/* Esse é o callback  que é chamado sempre que o algoritmo encontra
 * uma solução inteira;
 */
ILOLAZYCONSTRAINTCALLBACK2(SubtourEliminationCallback, Edges, x, IloNum, tol)
{
    // cerr << "Get in callback \n";
	IloInt i, j;
	IloInt n	= x.getSize();
	IloEnv env = getEnv();
	IloNumArray2 sol(env, n);
	for (i = 0; i < n; i++)
	{
		sol[i] = IloNumArray(env, x[i].getSize());
		getValues(sol[i], x[i]);
	}

    // Initialize Union-Find
    vector<int> parent(n);
    vector<int> rank(n);
    for(int i=0;i<n;++i) {
        parent[i] = i;
        rank[i] = 0;
    }

    int component = -1;
    for(int i=0;i<n;++i) 
        for(int j=0;j<sol[i].getSize();++j) 
            if(sol[i][j] >= 1.0-tol) {
                int x = find(parent, i);
                int y = find(parent, i+j+1);

                Union(parent, rank, x, y);

                if(x == y) {
                    component = x;
                    goto add_clique;
                }
            }
 
add_clique:

    if ( component == -1 ) // No cicle
    {
        for (i = 0; i < n; i++)
            sol[i].end();
        sol.end();
        return;
    }

    // Create and add subtour constraint ---
    // No more than 'length-1' edges between members of the subtour
    int length=0;
    for(int i =0;i<n;i++)
        if(find(parent,i) == component)
            length++;

	IloExpr clique(env);
    for(int i=0; i<n;++i) {
        if(find(parent, i) != component) continue;

        for(int j=i+1; j<n; ++j) {
            if(find(parent, j) != component) continue;

            clique += x[i][j - i -1];
        }
    }

    // cerr << "Adicionando corte com tamanho " << length << endl;
    add(clique <= length-1).end();
    clique.end();

    for (i = 0; i < n; i++)
        sol[i].end();
    sol.end();

    return;
}

int main(int argc, char** argv)
{
    try
	{
        ArgPack single_ap(argc, argv);

        string arq(ArgPack::ap().input_name);

        // Heuristic solution value
        int primal_bound = ArgPack::ap().primal_bound;
        // Memory limit
        long memory = ArgPack::ap().memory;
        // Time limit
        long time = ArgPack::ap().time;
        // L value
        int L = ArgPack::ap().L;
        // R value
        int R = ArgPack::ap().R;

        std::string input(arq);
        RunInstance(input, L, R, primal_bound, memory, time);
    }
    catch (IloException& e)
    {
		cerr << "Concert exception caught: " << e << endl;
	}
    catch (string& e)
    {
		cerr << e << endl;
	}
	catch (...)
	{
		cerr << "Unknown exception caught" << endl;
	}
	return 0;
}

void RunInstance(std::string & InstanceDir, int L, int R, int primal_bound, long memory, long time) {
	FInstance Instance = GetInstance(InstanceDir);
    // std::cout << Instance.Edges.size() << " " << Instance.Terminals.size() << std::endl;

    bool link_router = not (L==0 and R==0);
    cout << "Constraint link_router L=" << L << " R=" << R << endl;
    int min_W = INT32_MAX;
    for(auto &i : Instance.Terminals)
        if(i < min_W) min_W = i;

    int n = Instance.VertexCount;
 
    IloEnv env;
    IloModel model(env,"TCP");
    IloIntVarArray l(env, n, 0, 1);
    IloIntVarArray r(env, n, 0, 1); 
    IloArray<IloIntVarArray> x(env, n + 1); // Adding zero vertex
    for(int i=0; i <= n; i++)
        x[i] = IloIntVarArray(env, n - i, 0, 1);

    for(int i =0; i<= n; ++i)
        for(int j=0; j<n-i;++j)
            model.add(x[i][j]);

    for(int i=0;i<n;++i) {
        model.add(l[i]);
        model.add(r[i]);
    }

    IloExpr FO(env); // Objective function
    vector<vector<int>> neig(n + 1);
    for(int i =0;i<=n;++i)
        neig[i] = vector<int> (0);

    IloExpr sum(env);

    // Objective function
    for(auto &i : Instance.Edges) {
        int v1 = i[0];
        int v2 = i[1];
        int cost = i[2];
        if(v1 > v2) swap(v1, v2);

        FO += x[v1][v2 - v1 -1] * cost;

        neig[v1].push_back(v2);
        neig[v2].push_back(v1);

        sum += x[v1][v2 - v1 -1];
    }
    model.add(IloMinimize(env, FO));


    auto be = Instance.Terminals.begin();
    auto en = Instance.Terminals.end();
    for(int i =1; i<=n; ++i) {
        if(i == min_W) continue;

        auto p = find(be, en, i);
        if(p != en) // If i is terminal continue
            model.add(x[0][i-1] == 0);
        else
            sum += x[0][i-1];
    }
    // cout << "Min W = " << min_W << endl;
    sum += x[0][min_W - 1];
    model.add(sum == IloInt(n));


    for(int i=1; i<=n;++i) {
        auto p = find(be, en, i);
        if(p != en) continue; // If i is terminal continue

        for(auto j : neig[i]) {
            int v1 = i;
            int v2 = j;
            if(v1 > v2) swap(v1, v2);

            model.add(x[0][i-1] + x[v1][v2 - v1 - 1] <= IloInt(1));
        }
    }

    if(link_router)
    {
        // Constraint to active l and r
        for(int i=1; i<=n;++i) {
            IloExpr sum(env);
            int d_i = neig[i].size();
            for(int &j : neig[i])
                sum += i<j ?  x[i][j-i-1] : x[j][i-j-1];

            model.add(sum -  1 <= d_i * l[i-1]);
            model.add(sum -  2 <= d_i * r[i-1]);

        }
        
        model.add(IloSum(l) <= L);
        model.add(IloSum(r) <= R);
    }

    IloCplex cplex(model);

    IloNum tol = cplex.getParam(IloCplex::EpInt);
    IloCplex::Callback sec = cplex.use(
            SubtourEliminationCallback(env, x, tol ));

    cplex.setParam(IloCplex::PreInd, IloFalse);

    if(primal_bound != 0)
        cplex.setParam(IloCplex::CutUp, primal_bound);

    if(memory != 0)
    {
        cout << "Setting Memory Limit = " << memory << endl;
        cplex.setParam(IloCplex::TreLim, memory);
    }
    else
        cout << "No Memory Limit\n";


    if(time != 0)
    {
        cout << "Setting Time Limit = " << time << endl;
        cplex.setParam(IloCplex::TiLim, time);
    }
    else
        cout << "No time Limit\n";


    if ( cplex.solve() )
        env.out() << "Optimal solution cost "
            << cplex.getObjValue() << endl;

    
    IloNumArray2 sol(env, n);
    for (int i = 0; i < n; i++)
    {
        sol[i] = IloNumArray(env);
        cplex.getValues(sol[i], x[i]);
    }

    print_sol(sol, tol, Instance.Terminals);


}
//
TEdges LoadEdges(TEdges & InstData)
{
	TEdges Output;
	for (int i = 1; i <= InstData[0][1]; i++) {
		Output.push_back(InstData[i]);
	}
	return Output;
}

std::unordered_set<int> LoadTerminals(TEdges & InstData)
{
	std::unordered_set<int> Output;
	for (int i = InstData[0][1] + 2; i < InstData.size(); i++) {
		for (int Terminal : InstData[i]) {
			Output.insert(Terminal);
		}
	}
	return Output;
}
//
FInstance GetInstance(std::string const & InstanceDir)
{
	FInstance Instance;
	TEdges InstData = LoadInstance(InstanceDir);
	Instance.VertexCount = InstData[0][0];
	Instance.EdgeCount = InstData[0][1];
	Instance.Edges = LoadEdges(InstData);
	Instance.TerminalCount = InstData[Instance.EdgeCount + 1][0];
	Instance.Terminals = LoadTerminals(InstData);
	return Instance;
}
