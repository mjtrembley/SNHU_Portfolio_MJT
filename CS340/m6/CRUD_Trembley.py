from pymongo import MongoClient
from bson.objectid import ObjectId
from pprint import pprint

class CRUD_shelter(object):
    """CRUD operations for Animal collection in MongoDB """
   
    def __init__(self, username, password): #function to connect to database
        #inititializing MongoClient to access MongoDB databases and collections
        self.client = MongoClient('mongodb://%s:%s@localhost:46617' % (username, password)) #connects to database using local
        self.database = self.client['AAC'] #connect to correct database                      port number

    def create(self, data): #CRUD function to insert data into 'animal' collection
        if data is not None: 
            self.database.animals.insert(data)
            return True
        else:
            raise Exception("Nothing to save because data parameter is empty!")

    def read(self, data): #CRUD function to find and return document within 'animal' collection
        if data is not None:
            return self.database.animals.find(data, {"_id": False})
        else:
            raise Exception("Key/value pair not found!")
            return False

    def update(self, data, updateData): #CRUD function to update data and return the list of updated documents
        if data is not None:
            self.database.animals.update(data,updateData)
            return self.database.animals.find(updateData)
        else:
            raise Exception("Key/value pair not found!")
            return False
        
    def delete(self, deleteData, findData): #CRUD function to remove document from collection list
        if deleteData is not None:
            self.database.animals.remove(deleteData)
            return self.database.animals.find(findData)
        else:
            raise Exception ("Key/value pair not found!")
            return False
        
