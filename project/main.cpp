#include <iostream>
#include <ostream>
#include <fstream>
#include <cassert>
#include "graph.h"
#include <vector>

/**
  Struct point che implementa un punto 2D.

  @brief Struct point che implementa un punto 2D.
*/
struct point {
    int x; ///< coordinata x del punto
    int y; ///< coordinata y del punto
    point() : x(0), y(0) {}
    point(int xx, int yy) : x(xx), y(yy) {}
    bool operator == (const point& other) const{
        return ((x == other.x) && (y == other.y));
    }
};
/**
  Ridefinizione dell'operatore di stream << per un point.
  Necessario per l'operatore di stream della classe graph.
*/
std::ostream& operator<<(std::ostream& os, const point& p) {
    std::cout << "(" << p.x << "," << p.y << ")";
    return os;
}

/**
  Test dei metodi fondamentali

  @brief Test dei metodi fondamentali
*/
void test_metodi_fondamentali() {
    std::cout << "******** Test metodi fondamental del grafo di interi ********" << std::endl;
    std::cout << "******* test ctor *******" << std::endl;
    graph<int> g;
    assert(g.getNodes() == 0);
    std::cout << "******* test copy ctor *******" << std::endl;
    graph<int> copy(g);
    assert(copy.getNodes() == 0);
    std::cout << "******* test operator= *******" << std::endl;
    copy.add_node(1); //g ha 0 nodi e copy ne ha 1
    g = copy; // assegnamento
    assert(g.getNodes() == 1);
    std::cout << "******* test operator== *******" << std::endl;
    graph<int> ge;
    ge.add_node(1);
    assert(g == ge);
    std::cout << "******* test stampa *******" << std::endl;
    std::cout << ge << std::endl;
}

/**
  Test d'uso del grafo di interi

  @brief Test d'uso del grafo di interi.
*/
void test_uso() {
    std::cout << "******** Test uso del grafo di interi ********" << std::endl;
    graph<int> g2;
    int data[6] = {1,2,3,4,5,6};
    for (int i = 0, j = 1; i < 6; i++)
    {
     g2.add_node(j++);
    }
    std::cout << "******* stampa dopo inserimento nodi *******" << std::endl;
    std::cout << g2 << std::endl;
    g2.add_arc(1, 2);
    g2.add_arc(1, 3);
    g2.add_arc(2, 4);
    g2.add_arc(3, 4);
    g2.add_arc(3, 5);
    g2.add_arc(5, 5);
    g2.add_arc(4, 6);
    g2.add_arc(2, 6);
    std::cout << "******* stampa dopo inserimento archi *******" << std::endl;
    std::cout << g2 << std::endl;
    assert(g2.getArcs() == 8);
    assert(g2.getNodes() == 6);
    g2.remove_arc(2, 6);
    std::cout << "******* stampa dopo rimozione arco *******" << std::endl;
    std::cout << g2 << std::endl;
    assert(g2.getArcs() == 7);
    g2.remove_node(1);
    std::cout << "******* stampa dopo rimozione nodo *******" << std::endl;
    std::cout << g2 << std::endl;
    assert(g2.getNodes() == 5);
    assert(g2.getArcs() == 5);
}

void test_metodi_richiesti() {
    std::cout << "******** Test metodi richiesti sul grafo di interi ********" << std::endl;
    std::cout << "******* test metodi getArcs, getNodes, exists() *******" << std::endl;
    graph<int> gg;
    gg.add_node(1);
    gg.add_node(2);
    gg.add_arc(1, 2);
    assert(gg.getArcs() == 1);
    assert(gg.getNodes() == 2);
    assert(gg.exists(1) == true);
    assert(gg.exists(3) == false);
    gg.add_node(3);
    assert(gg.exists(3));// ora esiste
    std::cout << "******* test metodo connected() *******" << std::endl;
    assert(gg.connected(1, 2)); // 1 e 2 son connessi
    assert(gg.connected(1, 1) == false); // 1 non ha un cappio
    gg.add_arc(1, 1);
    assert(gg.connected(1, 1));//ora son connessi
}

/*
Test iteratori lettura del grafo
*/
void test_iteratori_int() {
    std::cout << "******** Test del const-iterator sul grafo di interi ********" << std::endl << std::endl;
    graph<int> git;
    std::cout << "L'TERATORE DEL GRAFO ITERA SUGLI IDENTIFICATIVI DEI NODI." << std::endl;
    //filling the graph
    int data[6] = { 1,2,3,4,5,6 };
    for (int i = 0, j = 1; i < 6; i++)
    {
        git.add_node(j++);
    }
    
    graph<int>::const_iterator start;
    graph<int>::const_iterator end;
    start = git.begin();
    end = git.end();
    assert(*start == 1); //il primo identificativo del grafo è 1

    for (int i = 0; start != end; start++, i++) { //testo che l'iteratore funzioni correttamente
        assert(*start == data[i]);
        std::cout << *start << "   OK." << std::endl;
    }
}

void test_add_nodes_int() {
    std::cout << "******** Test del metodo ADD_NODES sul grafo di interi ********" << std::endl;
    graph<int> gn;
    //utilizzo vector come supporto
    std::vector<int> v{ 1,2,3,4,5 }; 
    std::vector<int>::iterator inizio;
    std::vector<int>::iterator fine;
    inizio = v.begin();
    fine = v.end();
    // reimpio il grafo col metodo add nodes usando l'iteratore
    gn.add_nodes(inizio, fine);
     
    //verifico che tutto sia corretto
    graph<int>::const_iterator start;
    graph<int>::const_iterator end;
    start = gn.begin();
    end = gn.end();
    for (int i = 0; start != end; start++ , i++)
        assert(*start == v.at(i));
    std::cout << "grafo dopo add_nodes: " << std::endl << gn << std::endl;
}

void serializzazione_grafo_int() {
    std::cout << std::endl << "*** PROVA DI SERIALIZZAZIONE ***" << std::endl;
    graph<int> gs;

    //filling the graph
    int data[6] = { 1,2,3,4,5,6 };
    for (int i = 0, j = 1; i < 6; i++)
    {
        gs.add_node(j++);
    }

    // Usando operator<< possiamo scrivere su file il contenuto del grafo<int>.
    // E' importante scrivere operator<< in modo tale da spedire SOLO i dati
    // e niente altro.
    std::ofstream ofs;
    ofs.open("output.txt"); // il file è aperto come testo
    if (!ofs.is_open()) {
        std::cerr << "ERRORE NELLA'APERTURA DEL FILE" << std::endl;
        return;
    }
    ofs << gs;
    ofs.close();

    // Riapriamo il file e creiamo un secondo graph<int> con i valori salvati 
    std::ifstream ifs;
    graph<int> ne;
    ifs.open("output.txt");
    if (!ifs.is_open()) {
        std::cerr << "ERRORE NELLA'APERTURA DEL FILE" << std::endl;
        return;
    }
    int n;
    while (!ifs.eof()) {
        ifs >> n;
        ne.add_node(n);
        ifs.ignore(256, '\n');
    }
    
}

void test_char() {
    std::cout << std::endl << "*** TEST GRAFO DI CHAR ***" << std::endl;
    graph<char> gc;
    gc.add_node('a');
    gc.add_node('b');
    gc.add_node('c');
    gc.add_node('d');
    std::cout << "grafo dopo aggiunta nodi: " << std::endl << gc << std::endl;
    gc.add_arc('a', 'b');
    gc.add_arc('b', 'c');
    gc.add_arc('c', 'c');
    gc.add_arc('c', 'd');
    std::cout << "grafo dopo aggiunta archi: " << std::endl << gc << std::endl;
    assert(gc.getNodes() == 4);
    assert(gc.getArcs() == 4);
    gc.remove_arc('c', 'c');
    gc.remove_node('c');
    assert(gc.getNodes() == 3);
    assert(gc.getArcs() == 1);
    assert(gc.exists('a') == true);
    assert(gc.exists('c') == false);
    assert(gc.connected('a', 'b') == true);

    std::cout << "******** Test del metodo ADD_NODES sul grafo di char ********" << std::endl;
    graph<char> gn;
    //utilizzo vector come supporto
    std::vector<char> v{ 'a','b','c','d','e'};
    std::vector<char>::iterator inizio;
    std::vector<char>::iterator fine;
    inizio = v.begin();
    fine = v.end();
    // reimpio il grafo col metodo add nodes usando l'iteratore
    gn.add_nodes(inizio, fine);

    //verifico che tutto sia corretto
    graph<char>::const_iterator start;
    graph<char>::const_iterator end;
    start = gn.begin();
    end = gn.end();
    for (int i = 0; start != end; start++, i++)
        assert(*start == v.at(i));
    std::cout << "grafo dopo add_nodes: " << std::endl << gn << std::endl;
}

void test_grafo_punto() {
    std::cout << std::endl << "*** TEST TIPI CUSTOM:  GRAFO DI POINT ***" << std::endl;
    graph<point> p;
    point a(1, 1), b(1, 2), c(2, 4), d(2, 1);
    p.add_node(a);
    p.add_node(b);
    p.add_node(c);
    p.add_node(d);
    p.add_arc(a, b);
    p.add_arc(a, c);
    p.add_arc(c, d);
    
    std::cout << "grafo dopo aggiunta nodi ed archi: " << std::endl << p << std::endl;

    assert(p.getArcs() == 3);
    assert(p.getNodes() == 4);

    p.remove_arc(a, c);
    assert(p.getArcs() == 2);
    
    p.remove_node(c);
    assert(p.getNodes() == 3);
    assert(p.getArcs() == 1);

    std::cout << "grafo dopo rimozione nodi ed archi: " << std::endl << p << std::endl;

    assert(p.exists(a));
    assert(p.exists(c) == false);
    assert(p.connected(a, b));


    std::cout << "******** Test del metodo ADD_NODES sul grafo di POINT ********" << std::endl;
    graph<point> gp;
    //utilizzo vector come supporto
    std::vector<point> v{ a,b,c,d };
    std::vector<point>::iterator inizio;
    std::vector<point>::iterator fine;
    inizio = v.begin();
    fine = v.end();
    // reimpio il grafo col metodo add nodes usando l'iteratore
    gp.add_nodes(inizio, fine);

    //verifico che tutto sia corretto
    graph<point>::const_iterator start;
    graph<point>::const_iterator end;
    start = gp.begin();
    end = gp.end();
    for (int i = 0; start != end; start++, i++)
        assert(*start == v.at(i));
    std::cout << "grafo dopo add_nodes: " << std::endl << gp << std::endl;
}


int main() {

    //Test con grafo di interi: graph<int> 

    test_metodi_fondamentali();

    test_uso();

    test_metodi_richiesti();

    test_iteratori_int();

    test_add_nodes_int();

    serializzazione_grafo_int();

    //Test con grafo di char: graph<char>

    test_char();

    //Test con grafo di punti: graph<point> 

    test_grafo_punto();
}