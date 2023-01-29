from pymongo import MongoClient
from bson.objectid import ObjectId
from pprint import pprint

class CRUD_shelter(object):
    """CRUD operations for Animal collection in MongoDB """
   
    def __init__(self, username, password):
        #inititializing MongoClient to access MongoDB databases and collections
        self.client = MongoClient('mongodb://%s:%s@localhost:46617' % (username, password))
        self.database = self.client['AAC']

    def create(self, data):
        if data is not None:
            self.database.animals.insert(data)
            return True
        else:
            raise Exception("Nothing to save because data parameter is empty!")

    def read(self, data):
        if data is not None:
            return self.database.animals.find(data)
        else:
            raise Exception("Key/value pair not found!")
            return False

