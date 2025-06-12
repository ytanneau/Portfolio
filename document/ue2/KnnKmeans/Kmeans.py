from sklearn.datasets import fetch_openml, load_digits
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import random

def centre(nb):
    liste = []
    ligne = np.array(mnist.data)
    for i in range(nb):
        liste.append(x[np.random.randint(x.shape[0])])
    return liste

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

def cherchePlusProche(listeDistance):
    retoure = None
    for i in listeDistance:
        (indice, distance) = i
        if (retoure == None):
            retoure = indice
            distanceMini = distance
        if (distance < distanceMini):
            retoure = indice
            distanceMini = distance
    return retoure

def atributionPoint(listeCentre):
    retoure = []
    for i in range(len(listeCentre)) :
        retoure.append([])
    print("| 0")
    for coorLigne in range(x.shape[0]):
        if ((coorLigne+1)%10000 == 0):
            print("|", coorLigne+1)
        listeDistance = []
        contunLigne = x[coorLigne]
        for i in range(len(listeCentre)):
            contunLigneCentre = listeCentre[i]
            listeDistance.append((i ,calculDistance(2, contunLigne, contunLigneCentre)))
        retoure[cherchePlusProche(listeDistance)].append(coorLigne)

    return retoure

def moyenne(listeGroup):
    print("Group : ")
    listeCentre = []
    for group in listeGroup:
        print(len(listeCentre)+1, "/", len(listeGroup))
        point = []
        for numValeur in range(len(x[0])):
            somme = 0
            for coorLigne in group:
                contunLigne = x[coorLigne]
                somme += contunLigne[numValeur]
            point.append(int(somme/len(group)))
        listeCentre.append(point)
    return listeCentre

def afficher(liste):
    plt.axis("off")
    for i in liste:
        nbr = np.array(i)
        nbr_image = nbr.reshape(28, 28)
        plt.imshow(nbr_image, cmap=matplotlib.cm.binary)
        plt.show()
#------------------------------------------------------------------------

mnist = fetch_openml("mnist_784") 
mnist.target = mnist.target.astype(np.int8)

x = np.array(mnist.data)
y = np.array(mnist.target)

listeCentre = centre(10)#random.randint(0, x.shape[0]-1)

for i in range(1):
    print("Boucle", i+1,"/1")
    listeGroup = atributionPoint(listeCentre)
    listeCentre = moyenne(listeGroup)

print(listeCentre)
afficher(listeCentre)
#with open("test", "w") as filse :
    #filse.write(str(atributionPoint(listeCentre)))

# On peut également mélanger le tout pour ne pas tester toujours le même chiffre 
#rd = np.random.permutation(x.shape[0])
#x = x[rd]
#y = y[rd]


nbr = x[12]
# nbr correspond aux pixels de  l'image 28x28 = 784 

# On peut vérifier que le label y[12] correspond bien au nombre affiché