#include <iostream>
#include <string>
using namespace std;

template<typename V>
class MapNode{                                  //class for linked list
    string key;                                 //to store key
    V value;                                    //to store value
    MapNode * next;                             //to store next

    MapNode(string key, V value){               //constructor
        this->key = key;
        this->value = value;
        next = NULL;
    }

    ~MapNode(){                                 //destructor
        delete next;
    }

    template<typename T> friend class ourmap;                        //friend class assigned
};

template <typename V>
class ourmap{                                   //to store bucket array

    MapNode <V> ** buckets;                     //2D bucket array to store head pointers
                                                //of the linked list corresponding to each index

    int count;                                  //to store size or number of elements
    int numBuckets;                             //to store number of buckets for compression function

    public:
    ourmap(){
        count = 0;
        numBuckets = 5;
        buckets = new MapNode<V>*[numBuckets];  //dynamically allocated

        for(int i = 0; i<numBuckets; i++){
            buckets[i] = NULL;                  //initialising each head pointer to NULL
        }
    }

    ~ourmap(){
        for(int i = 0; i<numBuckets; i++){
            delete buckets[i];                  //delete each linked list
        }
        delete []buckets;                       //delete total bucket
    }

    int size(){                                 //return size of map
        return count;
    }

    int getValue(string key){                   //to search for key
        int bucketIndex = getBucketIndex(key);  //to get bucketIndex
        MapNode <V> * head = buckets[bucketIndex];  //head of LL
        while(head!=NULL){
            if(head->key==key){                     //if found return value
                return head->value;
            }
            head = head->next;
        }

        return -1;                              //not found
    }

    private:

    int getBucketIndex(string key){
        int hashCode = 0;
        int currentCoeff = 1;

        //using "abcd" = ('a'*p^3)+('b'*p^2)+('c'*p^1)+('d'*p^0)

        for(int i = key.length()-1; i>=0; i--){
            hashCode += key[i]*currentCoeff;
            hashCode = hashCode % numBuckets;           //make sure we don't go out of range
            currentCoeff *= 37;                         //p = 37
            currentCoeff = currentCoeff % numBuckets;   //make sure we don't go out of range
        }

        return hashCode % numBuckets;
    }

    public:
    void insert(string key, V value){
        //to get the bucket index i.e. passing key through hash function
        int bucketIndex = getBucketIndex(key);
        //to insert the key-value pair in the linked list  corresponding to index obtained

        MapNode <V> * head = buckets[bucketIndex];
        while(head!=NULL){
            if(head->key==key){
                head->value = value;
                return;
            }

            head = head->next;
        }
        //otherwise creating a new node and inserting that node before head son making it
        //as the head nd marking the bucket index to this node as the new head
        head = buckets[bucketIndex];
        MapNode<V>* node = new MapNode <V> (key,value);
        node->next = head;
        buckets[bucketIndex] = node;
        count++;
        //now we will check the load factor after insertion
        double loadFactor = (1.0*count)/numBuckets;
        if(loadFactor>0.7){
            rehash();
        }
    }

    V removeKey(string key){
        int bucketIndex = getBucketIndex(key);  //to get bucketIndex
        MapNode <V> * head = buckets[bucketIndex];  //head of LL
        MapNode <V> * prev = NULL;
        while(head!=NULL){
            if(head->key==key){                     
               if(prev==NULL){
                   buckets[bucketIndex] = head->next;
               } else {
                   prev->next = head->next;     //connecting previous to head's next pointer
               }

               V value = head->next;
               head->next = NULL;               //before calling delete over it, in order to avoid
                                                //complete linked list deletion, we have to assign it's next to NULL

                delete head;
                count--;                         //reducing the size

                return value;                   //return the value stored at delted node
            }
            prev = head;
            head= head->next;
        }

        return -1;                              //not found
    }
    
    double getLoadFactor(){
        return (1.0*count)/numBuckets;
    }
    private:
    void rehash(){
        MapNode <V> ** temp = buckets;              //to store the old bucket
        buckets = new MapNode <V> *[2*numBuckets];  //doubling the size
        for(int i = 0; 2*numBuckets; i++){
            buckets[i] = NULL;                      //initialising each head pointer to NULL
        }
        int oldBucketCount = numBuckets;            
        numBuckets *=2;                             //updating new size
        count = 0;
        for(int i = 0; i<oldBucketCount; i++){
            MapNode <V> * head = temp[i];
            while(head!=NULL){
                string key = head->key;
                V value = head->value;
                insert(key,value);                  //inserting each value of old bucket in new one
                head= head->next;
            }
        }
        //deleting the old bucket
        for(int i = 0; i<oldBucketCount; i++){
            MapNode<V> * head = temp[i];
            delete head;
        }
        
        delete[]temp;
    }
};
int main()
{
    ourmap <int> map;
    for(int i = 0; i<10; i++){
        char x = '0' + i;
        string key = "abc";
        key = key + x;
        int value = i + 1;
        map.insert(key,value);
        cout<<map.getLoadFactor()<<endl;
    }
    
    for(int i = 0; i<10; i++){
        char x = '0' + i;
        string key = "abc";
        key = key + x;
        cout<<key<<" "<<map.getValue(key)<<endl;
        
    }
    
    cout<<map.size();
    
    
}
