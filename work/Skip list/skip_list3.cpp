#ifndef  LINKEDPTRSKIPLIST_INC
#define  LINKEDPTRSKIPLIST_INC

#define	DEBUGSKIPLIST	1

#include <cstdlib>;
#include <string>;
#include <iostream>;
using namespace std;

// The Maximum number of layers the skip list can reach
const int SKIPLIST_MAX_HEIGHT = 24;

/*
 * =====================================================================================
 *       Struct:  Node
 *  Description:  This struct represents a single object stored in the Skiplist.
 *  		  Each Object has:
 *  		  1. Next: An array of pointers to other nodes, each cell in the 
 *  		  array representing a single corresponding layer.
 *  		  2. Height: The height of the layer array.
 *  		  3. Key: The string key representing this object.
 *  		  4. Object: The actual Object pointer
 * =====================================================================================
 */
template < class LinkedType >
struct Node
{
	int 				Height;
	string 				Key;
	Node<LinkedType>** 	Next;
	LinkedType* 		Object;
};

/*
 * =====================================================================================
 *        Class: LinkedPtrSkipList
 *  Description: A Linked Skip List implementation, designed to allow a string key associated
 *               with a pointer to an object by a specific template specified class. 
 * =====================================================================================
 */
template < class LinkedType >
class LinkedPtrSkipList
{
	public:

		LinkedPtrSkipList () 
		{
			this->mHeight=0;
			this->mpHead = NULL;	
			this->mCounter = 0;
		}


		LinkedPtrSkipList ( const LinkedPtrSkipList &other ) 
		{
			this->mHeight=0;
			this->mpHead = NULL;	
			this->mCounter = 0;

			this->mirror(&other);
		}

		LinkedPtrSkipList ( const LinkedPtrSkipList* other ) 
		{
			this->mHeight=0;
			this->mpHead = NULL;	
			this->mCounter = 0;

			this->mirror(other);
		}

		~LinkedPtrSkipList () 
		{
			this->clear();
		}


		/*-----------------------------------------------------------------------------
		 *  Iteration support
		 *-----------------------------------------------------------------------------*/

		class skipListPair : public pair<string,LinkedType*> {
			friend class LinkedPtrSkipList;
		public:
			skipListPair(string Key, LinkedType* Object) : pair<string,LinkedType*>(Key,Object) {}
			string Key() { return this->first;}
			LinkedType* Object() {return this->second;}
		};

		class iterator {
			friend class LinkedPtrSkipList;
			Node<LinkedType>* mNode;
		public:
			iterator(Node<LinkedType>& node) : mNode(&node) {}
			iterator(Node<LinkedType>* node = NULL) : mNode(node) {}
			
			iterator& operator++() 
			{ 
				mNode=mNode->Next[0]; 
				return *this; 
			}

			iterator operator++(int a) 
			{ 
				iterator retval = *this; 
				++*this; 
				return retval; 
			}

			skipListPair operator*() const 
			{ 
				if(mNode == NULL) {
					skipListPair p("",NULL);
					return p;
				}
				skipListPair p(mNode->Key,mNode->Object);
				return p;
			}

			bool operator==(const iterator& rhs) const 
			{ 
				return (mNode == rhs.mNode); 
			}

			bool operator!=(const iterator& rhs) const 
			{ 
				return (mNode != rhs.mNode); 
			}
		};

		iterator begin() const { 
			if(this->mHeight == 0) {
				return NULL;
			}

			return iterator(this->mpHead[0]); 
		}

		iterator end() const { 
			return NULL;
		}
			

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  operator =
		 *  Description:  This method allows us to create a copy of another Skip List.
		 *  		  This copying process copies the Key-Value pairs in the existing list
		 *  		  but does not duplicate the skip structure. Rather it redispurses
		 *  		  the objects using it's Insert method to evenly redistribute them.
		 *  		  This is useful in cases where a list may be unevenly distrubuted
		 *  		  following multiple removals and insertions.
		 * =====================================================================================
		 */
		LinkedPtrSkipList& operator = ( const LinkedPtrSkipList* other )
		{
			this->mirror(other);
		}

		LinkedPtrSkipList& operator = ( const LinkedPtrSkipList& other )
		{
			this->mirror(&other);

		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  size
		 *  Description:  This method returns the number of Objects stored in the Skip list.
		 * =====================================================================================
		 */
		int size() {
			return this->mCounter;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  remove
		 *  Description:  This method removes an object from the List by its key and returns it. 
		 *  		  This method will not delete the Object itself, as the Skip list does 
		 *  		  not manage the memory state of the objects it stores in any way.
		 * =====================================================================================
		 */
		LinkedType* remove(string Key)
		{
			// Check if the list is empty
			if(this->mHeight == 0) {
				return NULL;
			}
			
			Node<LinkedType>* node = NULL; 

			// Temporary array to hold pointers to the nodes leading the path to the node that needs deletion
			Node<LinkedType>** nodePath = this->tracePathToKey(Key,node,false);

			if(node == NULL || nodePath == NULL) {
				// If no node with this key was found or no path to it was found, then means there is no node to remove
				return NULL;
			}

			int numberOfLayersToRealign = node->Height;
			
			for(int layerIndex = 0; layerIndex < numberOfLayersToRealign; layerIndex++) {
				if(nodePath[layerIndex] == NULL) {
					// the node being removed is first in this layer - we'll replace him
					// in the head with the node he is pointing to
					this->mpHead[layerIndex] = node->Next[layerIndex];
				} else {
					nodePath[layerIndex]->Next[layerIndex] = node->Next[layerIndex];
				}
			}

			// Now that we have removed the node from the skiplist we can delete it
			LinkedType* object = node->Object;

			// we don't want to delete the Next array, as the nodes it is pointing to are still in the list
			node->Next = NULL;
			delete node;
			this->mCounter--;

			return object; 
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  insert
		 *  Description:  This method inserts a new Object into the List. The object's retrieval
		 *  		  key must be specified as well.
		 * =====================================================================================
		 */
		void insert(string Key, LinkedType* Object)
		{

			// If this is the first node ever - just create it and insert it
			if(this->mHeight == 0) {

				this->init();
				
				// There are no nodes prior to this one
				this->mpHead[0] = createNode(Key,Object);
				this->mpTail == this->mpHead[0]; // the tail is the new element
				this->mHeight = 1;
			} else {
				Node<LinkedType>* node = NULL;

				// Temporary array to hold pointers to the nodes leading the path to the node that needs insertion
				Node<LinkedType>** nodePath = this->tracePathToKey(Key,node,true);

				if(node != NULL) {
					// If the ndoe is not NULL then the TracePathToKey method  found another node with the same value
					// We will simply replace that node's Object with the new one and the insertion action is complete
					node->Object = Object;
					return;
				}

				node = this->createNode(Key,Object,generateRandomHeight());
				
				int numberOfLayersToCopyFromPath = node->Height;
				if(node->Height > this->mHeight) {
					// this node has started a new layer in the skip list
					this->mpHead[this->mHeight] = node;
					this->mHeight++;
					numberOfLayersToCopyFromPath--; // don't exceed the size of the path array
				}

				
				for(int layerIndex = 0; layerIndex < numberOfLayersToCopyFromPath; layerIndex++) {
					if(nodePath[layerIndex] == NULL) {
						// insert the node before any other node in the current layer
						node->Next[layerIndex] = this->mpHead[layerIndex];
						this->mpHead[layerIndex] = node;
					} else {
						node->Next[layerIndex] = nodePath[layerIndex]->Next[layerIndex];
						nodePath[layerIndex]->Next[layerIndex] = node;
					}
				}

				if(node->Next[0] == NULL) {
					// if the next node on layer 0 is NULL that means this is the new tail
					this->mpTail = node;
				}
			}

			
			// increase the node count
			this->mCounter++;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  find
		 *  Description:  This method retrieves an Object which is stored in the Skip list using
		 *  		  it's key.
		 * =====================================================================================
		 */
		LinkedType* find(string Key) 
		{
			Node<LinkedType>* node = NULL;

			// Temporary array to hold pointers to the nodes leading the path to the node that needs insertion
			Node<LinkedType>** nodePath = this->tracePathToKey(Key,node,true);

			if(node != NULL) {
				// If the ndoe is not NULL then the TracePathToKey method  found another node with the same value
				// We will simply replace that node's Object with the new one and the insertion action is complete
				return node->Object;
			}

			return NULL;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  exists
		 *  Description:  This method checkes whether a particular Key coresponds to an Object
		 *  		  inside the List.
		 * =====================================================================================
		 */
		bool exists(string Key) {
			return (this->find(Key) != NULL);
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  clear
		 *  Description:  This method removes all the Objects from the Skip List. It does not
		 *  		  delete the Objects themselves, as the container does not manage the
		 *  		  object's state in memory in any way.
		 * =====================================================================================
		 */
		void clear() {

			if(this->mHeight > 0) {
				Node<LinkedType>* node = this->mpHead[0];

				while(node != NULL) {
					Node<LinkedType>* tempNode = node->Next[0];
					node->Object = NULL;
					delete node;
					node = tempNode;
				}

				for(int layerIndex = 0; layerIndex < this->mHeight; layerIndex++) {
					this->mpHead[layerIndex] = NULL;
				}

				this->mHeight = 0;
				this->mCounter = 0;
			}
		}
		
		void debugPrint(bool PrintAddress = false)
		{
			for(int n = this->mHeight-1; n >= 0; n--) {
				Node<LinkedType>* node = this->mpHead[n];
				cout << "|" << (n+1) << "|";
				while(node != NULL) {
					cout << "->" << node->Key;
					if(PrintAddress) {
						cout << " [" << &node->Object << "]";
					}
					node = node->Next[n];
				}

				cout << "-|" << endl;
			}
		}
	private:
		void init()
		{
			if(this->mpHead == NULL) {
				// Seed random bit generator
				srand ( time(NULL) );

				// Create head array to hold pointers to nodes
				this->mpHead = new Node<LinkedType>*[SKIPLIST_MAX_HEIGHT]; 
				for(int index = 0; index < SKIPLIST_MAX_HEIGHT; index++) {
					this->mpHead[index] = NULL;
				}
			}
		}

		void mirror(const LinkedPtrSkipList* other) {

			this->clear();
			
			if(other->mHeight > 0) {
				Node<LinkedType>* node = other->mpHead[0];
				while(node != NULL) {
					this->insert(node->Key,node->Object);
					node = node->Next[0];
				}
			}
		}

		Node<LinkedType>* createNode(string Key, LinkedType* Object, int Height = 1) {
			Node<LinkedType>* node = new Node<LinkedType>();
			node->Key = Key;
			node->Object = Object;
			node->Height = Height;
			node->Next = new Node<LinkedType>*[Height];
			for(int skipLayer = 0; skipLayer < Height; skipLayer++) {
				node->Next[skipLayer] = NULL;
			}

#ifdef DEBUGSKIPLIST
			cout << "Create " << Key << " with height " << Height << " and located at " << &node << endl;
#endif
			return node;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  tracePathToKey
		 *  Description:  This method is the heart of the SkipList algoritham. It recieves a Key
		 *  		  as input and returns an array in which each cell coresponds to a 
		 *  		  single layer of the Skip list. In each one of these layers the method
		 *  		  will place apointer to the node *which points to the node we are 
		 *  		  looking for* OR *which ponts to where that key should be inserted*.
		 *  		  That means that if key X should go imeditely after key Y on layer #2
		 *  		  then the array will contain a pointer in index 1 (arrays start at 0)
		 *  		  which points to node Y.
		 *  		  We use this in order to find nodes and to figure out where to insert them.
		 * =====================================================================================
		 */
		Node<LinkedType>** tracePathToKey(string Key, Node<LinkedType> *& NodeOfKeyValue = NULL, bool BreakOnFindingKeyInList = false)
		{

			// Temporary array to hold pointers to the nodes leading the path to the node that needs insertion
			Node<LinkedType>** nodePath = new Node<LinkedType>*[this->mHeight];

			Node<LinkedType>* currentNode = NULL;
			int layerIndex = this->mHeight-1;
			while(currentNode == NULL && layerIndex >= 0) {
				if(this->mpHead[layerIndex] != NULL) {
					int currentKeyComparison = this->mpHead[layerIndex]->Key.compare(Key);
#ifdef DEBUGSKIPLIST
					cout << this->mpHead[layerIndex]->Key << " compareed to " << Key << " is " << currentKeyComparison << endl;
#endif
					if(currentKeyComparison < 0) {
						// the first node in this layer is smaller than the Key
						currentNode = this->mpHead[layerIndex];
						nodePath[layerIndex] = currentNode;
					} else if(currentKeyComparison == 0) {
						// found another node with the same Key - return it using the out parameter and end the function's logic
						NodeOfKeyValue = this->mpHead[layerIndex];
						if(BreakOnFindingKeyInList) {
							return NULL;
						} else {
							// if we don't intend on stopping the search (which means we still want to compute
							// the whole path, we'll place NULL in the current layer's spot - this denotes that the head points
							// to the requested Key
							nodePath[layerIndex] = NULL;
							layerIndex--;
						}
					} else {
						// the first node in the layer is larger and the new Node should be inserted before it
						// We will place NULL in the path's index to signify that this layer should point at the
						// new node once it is inserted.
						nodePath[layerIndex] = NULL;
						
						// move down one layer and keep on searching for a value smaler than what we are inserting
						layerIndex--;
					}
				}

			}

			// find the path to the new node's proper place
			// Note that this loop uses the previously set layerIndex which will already point
			// to the layer index that contains the currentNote
			for(/* No initialization, we reuse the layerIndex variable */; layerIndex >= 0 && currentNode != NULL; layerIndex--) {
				while(currentNode->Next[layerIndex] != NULL && currentNode->Next[layerIndex]->Key.compare(Key) < 0) {
#ifdef DEBUGSKIPLIST
					cout << currentNode->Next[layerIndex]->Key << " is smaller than " << Key << endl;
#endif
					currentNode = currentNode->Next[layerIndex];
				}

				if(currentNode->Next[layerIndex] != NULL && currentNode->Next[layerIndex]->Key.compare(Key) == 0) {
					// found another node with the same Key - return it using the out parameter and end the function's logic
					NodeOfKeyValue = currentNode->Next[layerIndex];
#ifdef DEBUGSKIPLIST
					cout << "Replaced Object under key " << Key << endl;
#endif
					if(BreakOnFindingKeyInList) {
						return NULL;
					}
				}
				nodePath[layerIndex] = currentNode;
			}

			return nodePath;
		}

		/* 
		 * ===  FUNCTION  ======================================================================
		 *         Name:  GenerateRandomHeight		 
		 *  Description:  The Linked list's power is rooted in the statistical nature by which
		 *  			new layers are inserted into the linked lists.
		 *  		  This method calculats a random height between 0 and the height of the
		 *  		  list. If its heigher than the current height a new layer will be added.
		 * =====================================================================================
		 */
		int generateRandomHeight()
		{
			// initialize the random bits
			static int bitsUpperBd = 0;
			static int bits;

			if (bitsUpperBd == 0) {
				// RAND_MAX is a constant defined in <cstdlib>. 
				// Its default value may vary between implementations but it is granted to be at least 32767.
				bitsUpperBd = RAND_MAX;

				// generate a random int value
				bits = rand();
			}

			// cycle through bits untill we reach a '1' bit, which
			// signifies the end of a cycle or we pass the current height
			int nextLevel = this->mHeight;

			// only allow the next level to be higher than the current highest
			// if we haven't yet reached the maximum height
			if(this->mHeight != SKIPLIST_MAX_HEIGHT) {
				nextLevel = this->mHeight + 1;
			}

			int b = 0;
			int height;
			for(height = 0; height < nextLevel  && b != 1; height++) {
				// compare the first bit. If it is '1' then we will end the loop here
				b = bits & 1;

				// shift the bits in 'bits' & the bits upper bound so that next time
				// we will get a different result
				bits >>= 1;
				bitsUpperBd >>= 1;
			}

			return height;
		}

		int 				mHeight;
		int					mCounter;
		Node<LinkedType>**	mpHead;
		Node<LinkedType>*	mpTail;  // need tail for iterator
}; /* -----  end of template class LinkedPtrSkipList  ----- */


#endif   /* ----- #ifndef LINKEDPTRSKIPLIST_INC  ----- */