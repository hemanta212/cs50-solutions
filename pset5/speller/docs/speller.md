
# Table of Contents

1.  [Model](#org27807f5)
2.  [Some Explorations](#org9c3d12d)
    1.  [0 points to what?](#orga50c5d2)
    2.  [NULL points to what?](#org6270605)
3.  [Implementing a Simple hash table](#org8d90345)
    1.  [Headers](#orgae59858)
    2.  [Hash table functions](#org424bd2a)
    3.  [Hash function](#orgad0bf53)
    4.  [Storing](#org2ace130)
    5.  [Retrieving](#orgeff16ea)
    6.  [Free the table](#orgef936d0)
4.  [CS50 Speller](#org7c69209)
    1.  [Some More explorations](#org61ad7ae)
        1.  [Reading the dictionary file](#org5203066)

Hash map implementation.

Spin Off course worth looking: [jamesroutley/Write-A-Hash-Table](https://github.com/jamesroutley/write-a-hash-table)


<a id="org27807f5"></a>

# Model

We're building a hash table with index and the keys it points to.

-   The index are inbuilt arrays of pointers.
-   Each of the index(key) points to a linked list.
-   The linked list itself is a list of custom struct called node

<table border="2" cellspacing="0" cellpadding="6" rules="groups" frame="hsides">


<colgroup>
<col  class="org-left" />

<col  class="org-left" />
</colgroup>
<tbody>
<tr>
<td class="org-left">Key</td>
<td class="org-left">Value</td>
</tr>


<tr>
<td class="org-left">p1</td>
<td class="org-left">n1-&gt;n2-&gt;n3</td>
</tr>


<tr>
<td class="org-left">0</td>
<td class="org-left">(null)</td>
</tr>


<tr>
<td class="org-left">0</td>
<td class="org-left">(null)</td>
</tr>


<tr>
<td class="org-left">p2</td>
<td class="org-left">n1</td>
</tr>
</tbody>
</table>

-   p1,p2 are the keys, the pointers to the linked list or first item of linked list.
-   n1, n2 are the nodes the members of linked list chained together

We basically do following things

-   initialize an array using malloc(so we can resize on demand)
-   with calloc initialize it to 0 so we know its not pointing to garbage value
-   We fill the the 0'd keys with new linked list
-   For non-zero key we can be sure that it at least has a one item linked list so we append to the first poistion of list.


<a id="org9c3d12d"></a>

# Some Explorations


<a id="orga50c5d2"></a>

## 0 points to what?

-   What does a 0'd key points to ie. if a pointer is 0 instead of valid memeory addreess, what do we get when dereferencing it?
    
        char* a = 0;
        printf("%s", a);
    
    It gives (null)? honestly, this is my first time seeing this output.


<a id="org6270605"></a>

## NULL points to what?

-   Lets try dereferencing the NULL
    
        char* a = NULL;
        printf("%s", a);
    
    NULL points to NULL


<a id="org8d90345"></a>

# Implementing a Simple hash table

Lets as an simple excersice implement a hash table. 
The each key in our index array of table will just point to a node instead of a linked list.
We'll not handle collision for this.


<a id="orgae59858"></a>

## Headers

We import

-   stdlib for malloc, calloc, free etc
-   string lib for comparing strings
-   math for calclulating the hash with pow

Similarly, we use some prime number for size of our array.

Then the node struct we want to store, it just stores key value string pairs.

    #include <stdlib.h>
    #include <math.h>
    #include <string.h>
    
    const int N = 51;
    
    typedef struct{
      char* key;
      char* value;
    } node;


<a id="org424bd2a"></a>

## Hash table functions

Our hash table provides the following function as api to interact with

With these functions we can store and retrieve string key values from the table
Similarly the hash function takes a string and generates int from 0 to table size (here 51).

After we're done with the table, we just purge the memory allocated in the heap with a call to 'del<sub>table</sub>'

    void store(node** table, char* key, char* value);
    char* retrieve(node** table, char* key);
    long hash(char* word);
    void del_table(node** table);


<a id="orgad0bf53"></a>

## Hash function

Hash function maps a key to index of our table array.

Some basic properties of hash function

-   Generate a unique hash for words, small change in spelling ideally should yield substancially different hash
-   Consistent, this is very important as a hash table that generates same hash for a given string is useless.
-   Handle collision, though we may calculate substancially diffrent hash values for given set of words, in the end we have to reduce them to finite set of buckets or array size (here in our case 51) so the result of our hash function is guarenteed to at least repeat every 51 times when this happens it is considered a collision.

We'll leave collision handling for now however our hash function will have other proeprties.

There are numerous implementation of hash functions, our is very very basic one.
What we are doing is factoring each of the charectars in a word to generate a huge number so that even a single char change in a word will map to different huge number.

Then we reduce the number acc to size of our array to get the index to place this word on.

    int hash(char* word){
      unsigned long long hash_value = 0;
      const double PRIME_SEED = 7.0;
      int key_len = strlen(word);
      for (int i = 0; i < key_len; i++) {
        hash_value += pow(PRIME_SEED, word[i] / PRIME_SEED);
      }
      hash_value = hash_value % N;
      return (int)hash_value;
    }


<a id="org2ace130"></a>

## Storing

-   For storing we take a table, key and value.
-   Create a node item by initializing the struct with key and value.
-   Calculate the index to place the value in, with a call to hash(key).
-   Go to the index if it is NULL then place the newly created node item there.
-   if not null, then a node item already exists check its key
-   if the key matches, user wants to update the value of key so update it.
-   if the key doesnot match, we have a collision! we just print it and do nothing to handle it.
    (collision because different key(word) passed to hash function yielded same index)
    
        void store(node** table, char* key, char* value){
          long index = hash(key);
          node* item = malloc(sizeof(node *));
          item->key = key;
          item->value = value;
        
          if (table[index] == NULL){
            table[index] = item;
          }
        
          int key_match = strcmp(key, table[index]->key) == 0;
          if(key_match)
            table[index]->value = value;
          else
            printf("Collision!!");
        }


<a id="orgeff16ea"></a>

## Retrieving

For retrieving, we take a string key and the table to lookup on,

-   calculate the index using a call to hash(key)
-   see if the table[index] is NULL, if yes, user tried to retrieve a key without storing first.
-   See if the key of table[index] node item matches the key, if yes return
-   if it didn't match it was the case of collision, we dont handle that here :)
    
        char* retrieve(node** table, char* key){
          long index = hash(key);
          if (table[index] != NULL){
            int key_match = strcmp(key, table[index]->key) == 0;
            if(key_match)
              return table[index]->value;
          }
          return NULL;
        }


<a id="orgef936d0"></a>

## Free the table

Since we've used malloc and calloc calls to ask the compiler to allocate us some memory on heap we should return it with free.

Since we are using malloc on each node we create and calloc only once creating the table, we just go through each node and free it and at last free the table.

    void del_table(node** table){
      for(int i=0; i<N; i++){
        if (table[i] == NULL)
          continue;
        free(table[i]);
      }
      free(table);
    }


<a id="org7c69209"></a>

# CS50 Speller

So we are ready for tackling the speller assignment


<a id="org61ad7ae"></a>

## Some More explorations


<a id="org5203066"></a>

### Reading the dictionary file

Lets try to read a the short version of dictionary file 

-   We open the file read char by char and append to a word array
-   Each time we encounter '\n' char we wrap up the word and print it
-   We reset the word count and start overwriting the array again repeating step 1

    #include <stdio.h>
    
    #define FILENAME "small"
    
    int main(void) {
      FILE *file = fopen(FILENAME, "r");
      if (file == NULL)
        return 1;
    
      char word[100];
      int word_len = 0;
    
      // keep reading char by char to buf until fread returns 0
      for(char buf; fread(&buf, sizeof(char), 1, file) == 1; word_len++){
        if (buf == '\n'){
          // we've encounred end of line wrap up the word and print it
          word[word_len++] = '\0';
          printf("%s\n", word);
    
          // reset the counter set to -1 because for loop will increment at last
          word_len = -1;
          continue;
        }
        // if not end of line just keep adding char to make word
        else
          word[word_len] = buf;
      }
      puts("");
    
      return 0;
    }

