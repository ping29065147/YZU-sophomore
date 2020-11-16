// xtree internal header

#ifndef XTREE
#define XTREE

enum class Color{ Red, Black }; // colors for link to parent

template< typename Ty >
struct TreeNode
{
   using NodePtr = TreeNode *;
   using value_type = Ty;

   NodePtr    left;   // left subtree, or smallest element if head
   NodePtr    parent; // parent, or root of tree if head
   NodePtr    right;  // right subtree, or largest element if head
   Color      color;  // Red or Black, Black if head
   bool       isNil;  // true only if head (also nil) node
   value_type myval;  // the stored value, unused if head
};

// CLASS TEMPLATE TreeVal
template< typename Ty >
class TreeVal
{
public:
   using NodePtr = TreeNode< Ty > *;

   using value_type = Ty;
   using size_type  = size_t;

   TreeVal()
      : myHead( new TreeNode< value_type > ),
        mySize( 0 )
   {
      myHead->left = myHead;
      myHead->parent = myHead;
      myHead->right = myHead;
      myHead->color = Color::Black;
      myHead->isNil = true;
   }

   ~TreeVal()
   {
      clear( myHead->parent );
      delete myHead;
   }

   // Removes all elements from the set object (which are destroyed)
   void clear( TreeNode< value_type > *node )
   {
      if( !node->isNil ) // node is not an external node
      {
         clear( node->left );
         clear( node->right );
         delete node;
      }
   }

   // rebalance for insertion
   void reBalance( TreeNode< value_type > *node )
   {  // node->parent cannot be the root

	   if (node->parent->color == Color::Black) return;

	   TreeNode< value_type >* P = node->parent;
	   TreeNode< value_type >* G = P->parent;
	   TreeNode< value_type >* U = (P == G->left) ? G->right : G->left;

	   if (U->color == Color::Red) {
		   if (G != myHead->parent) G->color = Color::Red;
		   P->color = Color::Black;
		   U->color = Color::Black;
		   reBalance(G);
	   }
	   else {
		   P->color = Color::Black;
		   G->color = Color::Red;
		   if (G->left == P && P->left == node) LLRotation(P);
		   else if (G->right == P && P->right == node) RRRotation(P);
	   }
   }

   // rotate right at g, where p = g->left and node = p->left
   //void set< Kty >::LLbRotation( TreeNode< value_type > *node )
   void LLRotation( TreeNode< value_type > *p )
   {
	   TreeNode< value_type >* G = p->parent;

	   G->left = p->right;
	   if (!p->right->isNil) p->right->parent = G;

	   if (G->parent->isNil) myHead->parent = p;
	   else G->parent->left == G ? G->parent->left = p : G->parent->right = p;
	   p->parent = G->parent;

	   p->right = G;
	   G->parent = p;
   }

   // rotate left at g, where p = g->right and node = p->right
   //void set< Kty >::RRbRotation( TreeNode< value_type > *node )
   void RRRotation( TreeNode< value_type > *p )
   {
	   TreeNode< value_type >* G = p->parent;

	   G->right = p->left;
	   if (!p->left->isNil) p->left->parent = G;

	   if (G->parent->isNil) myHead->parent = p;
	   else G->parent->left == G ? G->parent->left = p : G->parent->right = p;
	   p->parent = G->parent;

	   p->left = G;
	   G->parent = p;
   }

   // erase node provided that the degree of node is at most one
   void eraseDegreeOne( TreeNode< value_type > *node )
   {
	   TreeNode< value_type >* C = myHead;
	   if (!node->left->isNil) C = node->left;
	   else if (!node->right->isNil) C = node->right;

	   if (C == myHead) { // 0 child
		   if (node->color == Color::Black)
			   fixUp(node, node->parent);

		   if (node->parent->left == node) node->parent->left = C;
		   else if (node->parent->right == node) node->parent->right = C;

		   if (myHead->left == node) myHead->left = node->parent;
		   else if (myHead->right == node) myHead->right = node->parent;
	   }
	   else { // 1 child
		   if (node->parent->left == node) node->parent->left = C;
		   else if (node->parent->right == node) node->parent->right = C;

		   C->parent = node->parent;

		   if (myHead->left == node) myHead->left = C;
		   else if (myHead->right == node) myHead->right = C;

		   if (node->color == Color::Black)
			   fixUp(C, node->parent);
	   }


	   mySize--;
	   delete node;
   }

   // rebalance for deletion
   void fixUp( TreeNode< value_type > *N, TreeNode< value_type > *P )
   {
	   TreeNode< value_type >* S = (N == P->left) ? P->right : P->left;
	   
	   if (N->color != Color::Red) {
		   if (S == P->right) {
			   if (S->color == Color::Black && S->right->color == Color::Red) {
				   auto temp = S->color;
				   S->color = P->color;
				   P->color = temp;
				   S->right->color = Color::Black;
				   RRRotation(S);
			   }
		   }
		   else if (S == P->left) {
			   if (S->color == Color::Black && S->left->color == Color::Red) {
				   auto temp = S->color;
				   S->color = P->color;
				   P->color = temp;
				   S->left->color = Color::Black;
				   LLRotation(S);
			   }
		   }
	   }
	   N->color = Color::Black;
   }

/*
   // preorder traversal and inorder traversal
   void twoTraversals()
   {
      cout << "Preorder sequence:\n";
      preorder( myHead->parent );

      cout << "\nInorder sequence:\n";
      inorder( myHead->parent );
      cout << endl;
   }

   // preorder traversal
   void preorder( TreeNode< value_type > *node )
   {
      if( node != myHead )
      {
         cout << setw( 5 ) << node->myval << ( node->color == Color::Red ? "R" : "B" );
         preorder( node->left );
         preorder( node->right );
      }
   }

   // inorder traversal
   void inorder( TreeNode< value_type > *node )
   {
      if( node != myHead )
      {
         inorder( node->left );
         cout << setw( 5 ) << node->myval << ( node->color == Color::Red ? "R" : "B" );
         inorder( node->right );
      }
   }
*/

   NodePtr myHead;   // pointer to head node
   size_type mySize; // number of elements
};

// CLASS TEMPLATE Tree
template< typename Traits >
class Tree // ordered red-black tree for map/multimap/set/multiset
{
public:
   using value_type = typename Traits::value_type;

protected:
   using ScaryVal = TreeVal< value_type >;

public:
   using key_type      = typename Traits::key_type;
   using key_compare   = typename Traits::key_compare;

   using size_type       = size_t;

   Tree( const key_compare &parg )
      : keyCompare( parg ),
        scaryVal()
   {
   }

   ~Tree()
   {
   }

   // Extends the container by inserting a new element,
   // effectively increasing the container size by one.
   void insert( const value_type &val )
   {
	   TreeNode< value_type >* N = scaryVal.myHead->parent;
	   TreeNode< value_type >* P = N->parent;

	   while (N != scaryVal.myHead) {
		   if (N->myval == val) return;

		   P = N;
		   N = (keyCompare(val, N->myval)) ? N->left : N->right;
	   }

	   TreeNode< value_type >* newnode = new TreeNode< value_type >{ scaryVal.myHead, scaryVal.myHead, scaryVal.myHead, Color::Red, false, val };

	   if (scaryVal.mySize == 0) {
		   newnode->color = Color::Black;
		   scaryVal.myHead->parent = scaryVal.myHead->left = scaryVal.myHead->right = newnode;
		   scaryVal.mySize++;
		   return;
	   }

	   keyCompare(val, P->myval) ? P->left = newnode : P->right = newnode;
	   newnode->parent = P;

	   if (keyCompare(val, scaryVal.myHead->left->myval)) scaryVal.myHead->left = newnode;
	   else if (keyCompare(scaryVal.myHead->right->myval, val)) scaryVal.myHead->right = newnode;
	   
	   scaryVal.mySize++;
	   scaryVal.reBalance(newnode);
   }

   // Removes from the set container a single element whose value is val
   // This effectively reduces the container size by one, which are destroyed.
   // Returns the number of elements erased.
   size_type erase( const key_type &val )
   {
	   TreeNode< value_type >* N = scaryVal.myHead->parent;

	   while (N->myval != val) {
		   //if (N == scaryVal.myHead) return 0;

		   N = (keyCompare(val, N->myval)) ? N->left : N->right;

		   if (N == scaryVal.myHead) return 0;
	   }

	   if (!N->left->isNil && !N->right->isNil) {
		   TreeNode< value_type >* temp = N->right;
		   while (temp->left != scaryVal.myHead) temp = temp->left;
		   N->myval = temp->myval;
		   N = temp;
	   }

	   scaryVal.eraseDegreeOne(N);
	   return 1;
   }

private:
   key_compare keyCompare;
   ScaryVal scaryVal;
};

#endif // XTREE