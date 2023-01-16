from fileinput import close


FILENAME = "items.txt"
FREQUENCY_FILENAME = "frequency.dat"

def readFile(FILENAME):
    """
    Description: Gets the list of lines read in from the file in raw form.
    @param - the filename to read in (must be in same directory as the Source.cpp!)
    @return - the list of lines
    """

    with open(FILENAME) as file:
        lines = file.readlines()

    file.close()

    return lines


def getInventory():
    """
    Description: Get the inventory read in from the file.
    @return - dictionary of inventory in the form {item : frequency}
    """
    inventory = {}

    items = readFile(FILENAME)

    for item in items:
        item = item.lower().strip("\n")
        if item not in inventory:
            inventory[item] = 1
        else:
            inventory[item] += 1

    return inventory


def determineInventory():
    """
    Description: Prints the inventory dict.
    """
    inventory = getInventory()

    for key, value in inventory.items():
        print(key, value)


def determineFrequency(item_to_search):
    """
    Description: Checks the items frequency
    @param - the item to search.
    @return - the count of specified item.
    """

    inventory = getInventory()
    item_to_search = item_to_search.strip().lower()

    if item_to_search in inventory:
        count = inventory[item_to_search]
        return count
    else:
        return 0


def output():
    """
    Description: Creates or rewites a dat file contaning the inventory dict.
    """
    inventory = getInventory()
    f = open(FREQUENCY_FILENAME, "w")
    for key, value in inventory.items():
        f.write(str(key) + " " + str(value) + "\n")
    f.close()

