#ifndef __PUJ__GRAPH__HXX__
#define __PUJ__GRAPH__HXX__

#include "Graph.h"
#include <algorithm>
#include <queue>
#include <sstream>
#include <limits>

template < class V, class C>
PUJ::Graph<V,C>::Graph()
{

}

template < class V, class C>
PUJ::Graph<V,C>::~Graph()
{

}

template < class V, class C>
long PUJ::Graph<V,C>::
AddNode(V value)
{
	bool find = std::find(this->m_Vertices.begin(), this->m_Vertices.end(), value) != this->m_Vertices.end();

	if(!find)
	{
		this->m_Vertices.push_back( value );
		return this->m_Vertices.size() - 1;
	}
	else return -1;
}

template < class V, class C>
void PUJ::Graph<V,C>::
AddArc (long a, long b, C costo)
{
	if( !this->HasArc(a,b) )
	{
		this->m_Matrix[a][b] = costo;
	}
}

template < class V, class C>
bool PUJ::Graph<V,C>::
HasArc(long a, long b)
{
	typename Costs::iterator mIt = this->m_Matrix.find( a );

	if( mIt != this->m_Matrix.end())
	{
		typename std::map<long, TCost>::iterator mIt2 = (mIt->second).find( b );
		if( mIt2 != (mIt->second).end() )
			return true;
	}

	return false;
}

template < class V, class C>
V PUJ::Graph<V,C>::
GetNode(long a)
{
	if(a < this->m_Vertices.size())
	{
		return this->m_Vertices[a];
	}
}

template < class V, class C>
std::vector < V > PUJ::Graph<V,C>::
GetShortestPath ( long a, long b)
{
	std::vector<TValue> ret;
	if( a >= 0 && b >= 0 && a < this->m_Vertices.size() && b < this->m_Vertices.size())
	{
		long copB = b;
		std::vector<TValue> ret;
		std::vector<long> camino = this->GetDijkstra(a);
		while(camino[copB] != copB)
		{
			ret.push_back(this->m_Vertices[copB]);
			copB = camino[copB];
		}
		ret.push_back( this->m_Vertices[a] );
		return (ret);
	}
}

template < class V, class C>
std::vector < V > PUJ::Graph<V,C>::
GetShortestPathFW ( long a, long b )
{
	std::vector< std::vector<float> > costs;
	std::vector< std::vector<long> > paths;

	std::vector<V> path;

	this->GetFWarshall(costs, paths);

	if ( paths[a][b] == -1)
		return ( path ) ;
	path.push_back( this->m_Vertices[b] );

	long copB = b;
	while( copB != a)
	{
		copB = paths[a][copB];
		path.push_back( this->m_Vertices[copB] );
	}
	return path;
}

template < class V, class C>
std::vector < V > PUJ::Graph<V,C>::
GetShortestPath ( TValue a, TValue b, bool n)
{
	long v_a, v_b;
	v_a = v_b = -1;
	for( int i = 0; i < this->m_Vertices.size(); i++ )
	{
		if( this->m_Vertices[i] == a)
		{
			v_a = i;
		}
		if( this->m_Vertices[i] == b)
		{
			v_b = i;
		}
	}
	return ( this->GetShortestPath(v_a, v_b));
}


template < class V, class C>
std::vector< long > PUJ::Graph<V,C>::
GetDijkstra( long seed )
{
	std::vector < long > ret(this->m_Vertices.size( ), -1);
	std::vector< bool > marks( this->m_Vertices.size( ), false );

	std::vector < Triple<C> > triples;

	Triple<C> nueva;
	nueva.father = seed;
	nueva.son = seed;
	nueva.cost = 0;
	triples.push_back(nueva);

	while( !triples.empty( ) )
	{
		std::pop_heap(triples.begin(),triples.end(), comparador<C>());

		Triple<C> last = triples.back();

		triples.pop_back();

		if( marks[ last.son ] )
			continue;
		marks[ last.son ] = true;

		typename Costs::const_iterator mIt = this->m_Matrix.find( last.son );
		if( mIt != this->m_Matrix.end())
		{
			typename std::map<long, C>::const_iterator rIt = mIt->second.begin();
			for( ; rIt != mIt->second.end(); rIt++)
			{
				Triple<C> conec;
				conec.father = last.son;
				conec.son = rIt->first;
				conec.cost = last.cost + rIt->second;
				triples.push_back( conec );
				std::push_heap( triples.begin(), triples.end(), comparador<C>() );
			}

		}
		ret[ last.son ] = last.father;
	}

	return ret;
}

template < class V, class C>
std::list < V > PUJ::Graph<V,C>::
PrintLevels( long seed )
{
	std::list < TValue > ret;
	std::vector< bool > marks( this->m_Vertices.size( ), false );
	std::queue< long > q;
	q.push( seed );

	while( !q.empty( ) )
	{
		long node = q.front( );
		q.pop( );
		if( marks[ node ] )
			continue;
		marks[ node ] = true;
		for( long neigh = 0; neigh < this->m_Vertices.size( ); ++neigh )
		{
			if( this->HasArc( node, neigh ) )
			{
				q.push( neigh );
			}
		}
		ret.push_back( this->m_Vertices[node] );
	}
	return ret;
}

template < class V, class C>
void PUJ::Graph<V,C>::
GetFWarshall( std::vector< std::vector<C> >& costs, std::vector< std::vector<long> >& paths)
{
	typename Costs::iterator mIt = this->m_Matrix.begin();

	int i, j;
	i = 0;

	for(i = 0; i < this->m_Vertices.size(); i++)
	{
		std::vector<long> auxVec(this->m_Vertices.size(), -1);
		paths.push_back( auxVec );
	}

	for(i = 0; i < this->m_Vertices.size(); i++)
	{
		std::vector<C> auxVec(this->m_Vertices.size(), std::numeric_limits<C>::infinity());
		costs.push_back( auxVec );
	}

	i = 0;
	for(; mIt != this->m_Matrix.end(); mIt++)
	{
		i = mIt->first;
		j = 0;
		typename std::map<long, TCost>::iterator mIt2 = (mIt->second).begin();

		for(; mIt2 != (mIt->second).end(); mIt2++)
		{
			j = mIt2->first;
			costs[i][j] = mIt2->second;
			paths[i][j] = i;
		}
		costs[i][i] = 0;
	}

	for(int k = 0; k < this->m_Vertices.size(); k++ )
	{
		for(i = 0; i < this->m_Vertices.size(); i++ )
		{
			for(j = 0; j < this->m_Vertices.size(); j++ ) //i4, j0, k3
			{
				if( (costs[i][k] + costs[k][j]) < costs[i][j] )
				{
					costs[i][j] = costs[i][k] + costs[k][j];
					paths[i][j] = paths[k][j];
				}
			}
		}
	}
}

template < class V, class C>
long PUJ::Graph<V,C>::
GetSize()
{
	return ( this->m_Vertices.size() );
}

template < class V, class C>
void PUJ::Graph<V,C>::
PrintAsPNG( std::string file )
{
	std::stringstream str;
	str << "echo \"digraph G{";

	if(this->m_Vertices.size() == 1)
	{
		str << "\"" << this->m_Vertices[0] << "\"";
	}
	else
	{
		for(long i = 0; i < this->m_Vertices.size(); i++)
		{
			str << i << " [label=" << this->m_Vertices[i] << "]; ";
		}
		typename Costs::iterator mIt = this->m_Matrix.begin();

		long i, j;
		i = j = 0;
		std::vector<std::pair<long, long> > pairs;

		for( ; mIt != this->m_Matrix.end(); mIt++)
		{
			i = (mIt->first);
			typename std::map<long, TCost>::iterator mIt2 = (mIt->second).begin();
			for( ; mIt2 != (mIt->second).end(); mIt2++)
			{
				j = (mIt2->first);
				std::pair<long, long> pair(i,j);
				pairs.push_back(pair);
			}
		}

		mIt = this->m_Matrix.begin();

		for( ; mIt != this->m_Matrix.end(); mIt++)
		{
			i = (mIt->first);
			typename std::map<long, TCost>::iterator mIt2 = (mIt->second).begin();
			for( ; mIt2 != (mIt->second).end(); mIt2++)
			{
				j = (mIt2->first);
				std::pair<long, long> pareja(i,j);
				std::pair<long, long> swap;

				swap.first = pareja.second;
				swap.second = pareja.first;

				std::vector<std::pair<long, long> >::iterator it = std::find(pairs.begin(), pairs.end(), pareja);
				std::vector<std::pair<long, long> >::iterator itS = std::find(pairs.begin(), pairs.end(), swap);

				if(it!=pairs.end() && itS!=pairs.end())
				{
					pairs.erase(it);
					std::vector<std::pair<long, long> >::iterator itS = std::find(pairs.begin(), pairs.end(), swap);
					pairs.erase(itS);
					str << i << " -> " << j << " [dir=none, label=" << (mIt2->second) << "]; ";
				}
				else if(it!=pairs.end() || itS!=pairs.end())
				{
					str << i << "->" << j << " [label=" << (mIt2->second) << "]; ";
				}
			}
		}
	}
	str << "}\" | dot -Tpng > " << file;

	std::system( str.str( ).c_str( ) );
}

#endif
