def gift_inschrijven(tuple, dictionary):
    klas, bedrag = tuple
    if klas in dictionary:
        dictionary[klas] += bedrag
    else:
        dictionary[klas] = bedrag
    return dictionary