# Hashing

Implemented hash table containing objects which contain information about movies **original_language, original_title, release_date, revenue, runtime**. Data is hashed(indexed) based on name of the **original_title**, which is also used to as a key to access to the table. To solve collisions, method of open adressing is used.

Multiplication hashing methos is used for making hashes.

The size of the hash table is given when creating it and it can not be changed.

In class **HashTable()** there are following methods:
- **void findKey(string k, description& p)** : finds given key and change pointer to a string or put every value of pointer to -1 (if key is not found)
- **bool insertKey(Key k,description v)** : inserts the key and returns the status which is true for successfull insertion and false otherwise. It does not insert key that already exists
- **bool deleteKey(Key k)** : deletes the key fro hash table and return the status of deleting (true if deleting is successfull, false otherwise)
- **int avgAccessSuccess()** : average number of times accessing the table while successfully searching for a key
- **int avgAccessUnsuccess()** : average number of times accessing the table while unsuccessfully searching for a key
- **void resetStatistic()** : resets data used for counting number of times accessing the table
- **void clear()**: deletes all keys from the table
- **int tableSize()** : returns the size of the table
- **double fillRatio()** : returns the degree of occupancy(number between 0 and 1)

Abstract class declares public method used by HashTable() for determining next address when it comes to collision. Method returns new address where to find the key or where to place it using linear search with step s as follows:
**a = a + i*s**,
where **i** is the ordinal number of attempts and **a** is the original addres
