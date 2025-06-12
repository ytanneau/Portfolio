from math import *

def calculDistance(p, A, B):
    if (len(A) != len(B)):
        print("ERROR : taille de tablo différent")
        return -1
    else:
        somme = 0
        for i in range(len(A)):
            x = A[i]
            y = B[i]
            somme += abs(x-y)**p
        return somme**(1/p)
    
    
def trier(list):
    for i in range(len(list)):
            indice = 0
            for y in range(len(list)-i):
                (A,B) = list[y]
                (C,D) = list[indice]
                if (B > D):
                    indice = y
            permut = list[y]
            list[y] = list[indice]
            list[indice] = permut
    return list

def cherchePlusProche(liste, k, A, chois):
    indice = 0
    listeDistance = []
    listeCours = []
    retoure = 0
    for i in liste:
        listeDistance.append((indice, calculDistance(1,A,i)))
        indice += 1
    listeDistance = trier(listeDistance)
    for i in range(k):
        (indice, z) = listeDistance[i]
        listeCours.append(liste[indice])
    if (chois == "Régression" and len(listeCours) > 0):
        retoure = moyenne(listeCours)
    elif (chois == "Classification"):
        retoure = plusFrequent(listeCours)
    else:
        retoure = "ERROR"
    return retoure

def moyenne(liste):
    moyen = []
    for numValeur in range(len(liste[0])):
            somme = 0
            for ligne in liste:
                valeur = ligne[numValeur]
                somme += valeur
            moyen.append(int(somme/len(liste)))
    return moyen


def plusFrequent(liste):
    fois = 0
    liste = liste.copy()
    frequence = []
    print("liste :",liste)
    for i in liste:
        ajouter(i, frequence)
    print("fréquence : ", frequence)
    for i in frequence:
        (a,b) = i
        if (a > fois):
            fois = a
            retoure = b
    return retoure


def ajouter(nb, frequence):
    sortie = []
    if (len(frequence) == 0):
        frequence.append((1, nb))
    else:
        trouver = False
        for i in frequence:
            (nbFoix, valeur) = i
            if (valeur == nb):
                nbFoix += 1
                trouver = True
            frequence.append((nbFoix,valeur))
        if (trouver == False):
            frequence.append((1, nb))
    return frequence


A = [2,1,7]
B = [4,8]
liste = [(1,2,7), (1,4,6), (7,2,7), (9,5,1), (4,2,3), (7,4,4), (8,2,7), (3,5,4), (7,2,7)]
p = 2
print(cherchePlusProche(liste, 5, A, "Classification"))