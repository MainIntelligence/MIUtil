#pragma once
#include "Vector.h"

template <typename IDT>
struct IDGraph {
   Vector<Vector<IDT>> vecs;
   
   IDGraph(const unsigned int cap = 1) : vecs(cap) {};
   ~IDGraph() = default;
   
   Vector<IDT>& operator[](const unsigned int i) { return vecs[i]; }
   
   void AddNode();
   unsigned int TotalNodes() const noexcept { return vecs.Size(); }
   void AddNext( const IDT node, const IDT next );
   
   
   struct NodeData {
     Vector<IDT> & vec;
     
     NodeData( Vector<IDT> & nodevec ) : vec(nodevec) {}
     ~NodeData() = default;
     
     void operator=(NodeData&& swap);
     IDT& operator[](const unsigned int i) { return vec[i + 1]; }
     
     void Add(const IDT id); //no safety (make sure id not in nexts)
     unsigned int BinarySearch(const IDT item) const noexcept;
     void Remove(const IDT id);
   };
   
   struct Traverser {
     IDGraph<IDT> & graph;
     IDGraph<IDT>::NodeData cnxndat;
     unsigned int i = 0;
   
     bool Done() { return i == cnxndat.vec.Size() - 1; }
   
     Traverser( IDGraph<IDT> & graph ) : graph{graph}, cnxndat(graph[0]) {};
     ~Traverser() = default;
     
     void Goto(const unsigned int j) { cnxndat = IDGraph<IDT>::NodeData(graph[j]); i = 0; }
     void GoNext() { i++; }
     IDT& This() { return cnxndat[i]; }
     unsigned int Remaining() const noexcept { return cnxndat.vec.Size() - 1 - i; }
   };
   
};

//************* NODE DATA *******************
template <typename IDT>
void IDGraph<IDT>::NodeData::operator=(IDGraph<IDT>::NodeData&& swap) { vec = swap.vec; }


template <typename IDT>
void IDGraph<IDT>::NodeData::Add(const IDT item) {
  if (vec.Size() == vec.Capacity()) { vec.Resize( 2 * vec.Capacity() ); }
  //
  unsigned int i = vec.Size();
  for ( ; (i > 1) && (*(vec.data + i-1) > item) ; i--) {
     vec[i] = vec[i-1];
  }
  //printf("NodeData Add: %u %u\n", next, i);
  vec[i] = item;
  vec.numElements++;
}

//Replaced 'total' occurences with 'vec.Size() - 1', nexts with (*this)
template <typename IDT>
unsigned int IDGraph<IDT>::NodeData::BinarySearch(const IDT item) const noexcept {
   if ( item <= (*this)[0] ) { return 0; }
   if ( item >= (*this)[vec.Size()-2] ) { return vec.Size()-1; }
   unsigned int loweri = 0;
   unsigned int higheri = vec.Size() - 2;
   unsigned int i = (vec.Size() - 1)/2;
   while( higheri-loweri > 1 ) {
      if ( (*this)[i] == item ) { return i; }
      else if ( (*this)[i] > item ) { higheri = i; }
      else if ( (*this)[i] < item ) { loweri = i; }
      i = (loweri + higheri) / 2;
   }
   return i;
}

template <typename IDT>
void IDGraph<IDT>::NodeData::Remove(const IDT id) {
   unsigned int i = BinarySearch(id);
   if ((*this)[i] != id) { return; }
   
   for (; i < vec.Size() - 1 ; i++) {
     (*this)[i] = (*this)[i+1];
   }
   vec.numElements--;
}


//************** IDGraph *************************
template <typename IDT>
void IDGraph<IDT>::AddNode() {
   Vector<IDT> item (1);
   item.AddEnd( vecs.Size() );
   vecs.AddEnd( item );
}

template <typename IDT>
void IDGraph<IDT>::AddNext( const IDT node, const IDT next ) {
  NodeData handle (vecs[node]);
  handle.Add(next);
  /*
  Vector<IDT> & vecs = (*this)[node];
  if (vecs.Size() == vecs.Capacity()) { vecs.Resize( 2 * vecs.Capacity() ); }
  //
  unsigned int i = vecs.Size();
  for ( ; (i > 1) && (*(vecs.data + i-1) > next) ; i--) {
     vecs[i] = vecs[i-1];
  }
  //printf("NodeData Add: %u %u\n", next, i);
  vecs[i] = next;
  vecs.numElements++;*/
}

