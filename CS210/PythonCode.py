import re
import string
from collections import Counter

def wholeList(): #function to return a list of all items on groceryList.txt and number of times they appear
    with open('groceryList.txt', 'r') as f: #opens groceryList.txt
     groceryList = Counter([line.strip() for line in f]) #Counter formats groceryList, while counting number of occurrences.
     groceryList.keys()
     for key, value in groceryList.items(): #loop to format output
        print (key,"bought", value,"time(s)") 

def specificItem(): #function to return a singular item on groceryList.txt and the number of times it appeared, based on userInput
     with open('groceryList.txt', 'r') as f: #opens groceryList.txt
      groceryList = Counter([line.strip() for line in f]) #Counter formats groceryList, while counting number of occurrences.
      userInput = input()
      if userInput not in groceryList: #if-else statement to check for valid input
          print(userInput, "not found. Please try again.")
      elif userInput in groceryList:
          print(userInput, "purchased", groceryList[userInput], "time(s) today.")

def graphItems(): #function to write a new file, frequeuncy.dat while reading from groceryList.txt.
   with open('frequency.dat', 'w') as newFile: #opens new file for writing, frequency.dat
    with open('groceryList.txt', 'r') as f: #opens groceryList.txt
      groceryList = Counter([line.strip() for line in f]) #Counter formats groceryList, while counting number of occurrences.
      groceryList.keys()
      for key, value in groceryList.items(): #loop to write to new file, freqeuncy.dat
       newFile.write(key)
       newFile.write(" ")
       newFile.write(str(value))
       newFile.write("\n")
    newFile.close()
