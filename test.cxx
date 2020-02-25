#include <iostream>

#include "Graph.h"

int main(int argc, char** argv)
{
	PUJ::Graph<char,float> miPrimerGrafo;

	miPrimerGrafo.AddNode('A');
	miPrimerGrafo.AddNode('B');
	miPrimerGrafo.AddNode('C');
	miPrimerGrafo.AddNode('D');
	miPrimerGrafo.AddNode('E');
	miPrimerGrafo.AddNode('F');

	miPrimerGrafo.AddArc(0,2,6);
	miPrimerGrafo.AddArc(0,3,3);
	miPrimerGrafo.AddArc(1,0,3);
	miPrimerGrafo.AddArc(2,3,2);
	miPrimerGrafo.AddArc(3,2,1);
	miPrimerGrafo.AddArc(3,1,1);
	miPrimerGrafo.AddArc(4,1,4);
	miPrimerGrafo.AddArc(4,3,2);
	miPrimerGrafo.AddArc(0,5,1);
	miPrimerGrafo.AddArc(2,5,5);

	std::list<char> print = miPrimerGrafo.PrintLevels(0);
	std::list<char>::iterator pIt = print.begin();
	for(; pIt != print.end(); pIt++)
	{
		std::cout << *pIt << ", ";
	}

	miPrimerGrafo.PrintAsPNG("graph.png");

	std::vector< char > ret = miPrimerGrafo.GetShortestPath(0,1);
	std::cout << std::endl << std::endl;
	std::vector<char>::reverse_iterator rIt = ret.rbegin();
	for(; rIt < ret.rend(); rIt++)
	{
		std::cout << *rIt;
		if( (rIt+1) != ret.rend())
			std::cout << "->";
	}
	std::vector< char > ret2 = miPrimerGrafo.GetShortestPath('A','E',true);
	std::cout << std::endl << std::endl;
	std::vector<char>::reverse_iterator rIt2 = ret2.rbegin();
	for(; rIt2 < ret2.rend(); rIt2++)
	{
		std::cout << *rIt2;
		if( (rIt2+1) != ret2.rend())
			std::cout << "->";
	}
	std::cout << std::endl;

	std::vector<char> path = miPrimerGrafo.GetShortestPathFW(3,5);

	rIt2 = path.rbegin();
	for(; rIt2 < path.rend(); rIt2++)
	{
		std::cout << *rIt2;
		if( (rIt2+1) != path.rend())
			std::cout << "->";
	}
	std::cout << std::endl;

}
