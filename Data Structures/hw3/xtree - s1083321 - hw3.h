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
       if (node->parent->parent == myHead || node->parent == myHead || node->parent->color == Color::Black)
           return;
       
       char pt, grdpt, sibling;

       if (node->parent->left == node)
           pt = 'L';
       else
           pt = 'R';
       if (node->parent->parent->left == node->parent) {
           grdpt = 'L';
           if (node->parent->parent->right->color == Color::Black)
               sibling = 'B';
           else
               sibling = 'R';
       }
       else {
           grdpt = 'R';
           if (node->parent->parent->left->color == Color::Black)
               sibling = 'B';
           else
               sibling = 'R';
       }
       
       if (sibling == 'R') {
           if (node->parent->parent != myHead->parent)
               node->parent->parent->color = Color::Red;
           node->parent->parent->left->color = Color::Black;
           node->parent->parent->right->color = Color::Black;

           reBalance(node->parent->parent);
       }
       else {
           if (grdpt == 'L' && pt == 'L') {
               node->parent->color = Color::Black;
               node->parent->parent->color = Color::Red;
               LLRotation(node->parent);
           }
           else if (grdpt == 'L' && pt == 'R')
               LRRotation(node);
           else if (grdpt == 'R' && pt == 'R') {
               node->parent->color = Color::Black;
               node->parent->parent->color = Color::Red;
               RRRotation(node->parent);
           }
           else if (grdpt == 'R' && pt == 'L')
               RLRotation(node);
       }
   }

   // rotate right at g, where p = g->left and node = p->left
   //void set< Kty >::LLbRotation( TreeNode< value_type > *node )
   void LLRotation( TreeNode< value_type > *p )
   {
        p->parent->left = p->right;
        if (p->right != myHead)
           p->right->parent = p->parent;
        p->right = p->parent;
        if (p->parent->parent == myHead)
            myHead->parent = p;
        else {
            if (p->parent->parent->left == p->parent)
                p->parent->parent->left = p; 
            else
                p->parent->parent->right = p; 
        }
        p->parent = p->parent->parent;
        p->right->parent = p;
   }

   // LR rotation; p = g->left and node = p->right
   void LRRotation( TreeNode< value_type > *node )
   {
       RRRotation(node);
       node->color = Color::Black;
       node->parent->color = Color::Red;
       LLRotation(node);
   }

   // RL rotation; p = g->right and node = p->left
   void RLRotation( TreeNode< value_type > *node )
   {
       LLRotation(node);
       node->color = Color::Black;
       node->parent->color = Color::Red;
       RRRotation(node);
   }

   // rotate left at g, where p = g->right and node = p->right
   //void set< Kty >::RRbRotation( TreeNode< value_type > *node )
   void RRRotation( TreeNode< value_type > *p )
   {
       p->parent->right = p->left;
       if (p->left != myHead)
           p->left->parent = p->parent;
       p->left = p->parent;
       
       if (p->parent->parent == myHead)
           myHead->parent = p;
       else {
           if (p->parent->parent->right == p->parent)
               p->parent->parent->right = p;
           else
               p->parent->parent->left = p;
       }
       p->parent = p->parent->parent;
       p->left->parent = p;
   }

   // erase node provided that the degree of node is at most one
   void eraseDegreeOne( TreeNode< value_type > *node )
   {
       TreeNode< value_type >* child;

       if (node->left != myHead) {
           child = node->left;
           if (node->parent->left == node)
               node->parent->left = node->left;
           else
               node->parent->right = node->left;
           node->left->parent = node->parent;
           if (node == myHead->parent) {
               node->left->color = Color::Black;
               myHead->parent = node->left;
           }
       }
       else {
           child = node->right;
           if (node->parent->left == node)
               node->parent->left = node->right;
           else
               node->parent->right = node->right;
           node->right->parent = node->parent;
           if (node == myHead->parent) {
               node->right->color = Color::Black;
               myHead->parent = node->right;
           }
       }

       if (node->color == Color::Black)
           fixUp(child, node->parent);

      delete[] node;
   }

   // rebalance for deletion
   void fixUp( TreeNode< value_type > *N, TreeNode< value_type > *P )
   {
       TreeNode< value_type >* S;

       //Case 0: N is red, N is root
       while (N != myHead->parent && N->color == Color::Black) {

           if (N == N->parent->left)
               S = N->parent->right;
           else
               S = N->parent->left;

           //Case 1: sibling is red
           if (S->color == Color::Red) { 

               S->color = Color::Black;
               S->parent->color = Color::Red;
               if (S == N->parent->right) {
                   RRRotation(S);
                   S = N->parent->right;
               }
               else {
                   LLRotation(S);
                   S = N->parent->left;
               }
           }

           //Case 2: sibling is black, 2 children are black
           if (S->right->color == Color::Black && S->left->color == Color::Black) { 
               S->color = Color::Red;
               N = N->parent;
           }
           else{

               //Case 3: sibling is black, right child is black, left child is red
               if (S == N->parent->right && S->right->color == Color::Black) {
                   S->left->color = Color::Black;
                   S->color = Color::Red;
                   LLRotation(S->left);
                   S = N->parent->right;
               }
               else if (S == N->parent->left && S->left->color == Color::Black) {
                   S->right->color = Color::Black;
                   S->color = Color::Red;
                   RRRotation(S->right);
                   S = N->parent->left;
               }

               //Case 4: sibling is black, right child is red
               if (S == N->parent->right) {
                   S->color = N->parent->color;
                   N->parent->color = Color::Black;
                   S->right->color = Color::Black;
                   RRRotation(S);
                   N = myHead->parent;
               }
               else {
                   S->color = N->parent->color;
                   N->parent->color = Color::Black;
                   S->left->color = Color::Black;
                   LLRotation(S);
                   N = myHead->parent;
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
       TreeNode<value_type>* temp = new TreeNode<value_type>;
       temp->isNil = 0;
       temp->myval = val;
       temp->right = temp->left = temp->parent = scaryVal.myHead;

       if (scaryVal.mySize == 0) {
           temp->color = Color::Black;
           scaryVal.myHead->parent = scaryVal.myHead->left = scaryVal.myHead->right = temp;
           scaryVal.mySize++;
           return;
       }

       temp->color = Color::Red;
       TreeNode<value_type>* current = scaryVal.myHead->parent;
       TreeNode<value_type>* p = current;

       while (current != scaryVal.myHead) {
           if (val == current->myval)
               return;
           else if (keyCompare(val, current->myval)) {
               p = current;
               current = current->left;
           }
           else{
               p = current;
               current = current->right;
           }
       }
       
       if (keyCompare(val, p->myval)) {
           p->left = temp;
           if (keyCompare(temp->myval, scaryVal.myHead->left->myval))
               scaryVal.myHead->left = temp;
       }
       else {
           p->right = temp;
           if (keyCompare(scaryVal.myHead->right->myval, temp->myval))
               scaryVal.myHead->right = temp;
       }
       temp->parent = p;
       scaryVal.mySize++;

       scaryVal.reBalance(temp);
   }

   // Removes from the set container a single element whose value is val
   // This effectively reduces the container size by one, which are destroyed.
   // Returns the number of elements erased.
   size_type erase( const key_type &val )
   {
       TreeNode<value_type>* current = scaryVal.myHead->parent;

       while (val != current->myval) {
           if (current == scaryVal.myHead)
               return 0;
           else if (keyCompare(val, current->myval))
               current = current->left;
           else
               current = current->right;
       }

       TreeNode<value_type>* del = current;

       //0 child
       if (current->left == scaryVal.myHead && current->right == scaryVal.myHead) { 
           if (current->color == Color::Black)
               scaryVal.fixUp(current, current->parent);

           if (del->parent->left == del)
               del->parent->left = scaryVal.myHead;
           else
               del->parent->right = scaryVal.myHead;

           if (scaryVal.myHead->left == del)
               scaryVal.myHead->left = del->parent;
           else if (scaryVal.myHead->right == del)
               scaryVal.myHead->right = del->parent;

           delete[] del;
       }  //1 child
       else if (current->left == scaryVal.myHead || current->right == scaryVal.myHead) 
           scaryVal.eraseDegreeOne(current);
       else { //2 child

           del = current->right;
           while (del->left != scaryVal.myHead)
               del = del->left;

           current->myval = del->myval;
           current = del;

           if (del->left != scaryVal.myHead || del->right != scaryVal.myHead)
               scaryVal.eraseDegreeOne(current);
           else {
               if (current->color == Color::Black)
                    scaryVal.fixUp(current, current->parent);

               if (del->parent->left == del)
                   del->parent->left = scaryVal.myHead;
               else
                   del->parent->right = scaryVal.myHead;

               if (scaryVal.myHead->left == del)
                   scaryVal.myHead->left = del->parent;
               else if (scaryVal.myHead->right == del)
                   scaryVal.myHead->right = del->parent;

               delete[] del;
           }

       }

       scaryVal.mySize--;
       return 1;
   }

private:
   key_compare keyCompare;
   ScaryVal scaryVal;
};

#endif // XTREE