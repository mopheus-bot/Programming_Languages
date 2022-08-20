import re
import string

#initialize dictionary for products and their frequencies
productsDict = {}


# opens file in read mode, creating dictionary with product as key and frequency as value
# closes file after use
def AnalyzeData ():
    salesFile = open ('PurchasedProducts.txt', 'r')

    for currProduct in salesFile:
        currProduct = currProduct.replace('\n', '')
        if (currProduct in productsDict):
            productsDict[currProduct] += 1
        else:
            productsDict[currProduct] = 1
    salesFile.close()


# takes a line from C++ and formats it to print
def PrintAllItems():
    AnalyzeData()
    for currProduct in productsDict:
        print (currProduct.rjust(16, ' '), end = ':')
        print ("{:4d}" .format(productsDict[currProduct]))
    print ("\n\n")


# returns the frequency of selected product that is passed into function
def ReturnSpecificItem(product):
    AnalyzeData()
    product = product.capitalize()              # makes sure the first letter of user entered word is capitalized
    if (product in productsDict):
        return productsDict[product]
    else:
        return 0


# creates/opens output file in write mode, rewriting any currently existing data
# outputs a list of the products and their frequencies
def CreateOutputFile():
    AnalyzeData()
    outFile = open ('frequency.dat', 'w')

    for currProduct in productsDict:
        outFile.write("{} {}\n" .format(currProduct, productsDict[currProduct]))

    outFile.close()