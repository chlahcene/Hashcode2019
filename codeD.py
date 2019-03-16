# input files
file = ["a_example.txt", "b_lovely_landscapes.txt", "c_memorable_moments.txt", "d_pet_pictures.txt",
        "e_shiny_selfies.txt"]
# output files
o = ["a", 'b', 'c', 'd', "e"]
# for each file :
ll = 3 #file D

# reading of the file
f = open(file[ll], "r")
s = f.read()
s = s.split("\n")
n = int(s[0])  # The total number of the pictures
f.close()
H = []  # a list for the horizontal photos
V = []  # a list for the vertical photos

for i in range(n):  # for each photo we put it in its place
    ss = s[i + 1].split()
    b = []
    type = ss[0]  # Get the type of the photo
    ens = set()  # The set which will contain the tags of the photo
    b.append(i)
    for j in range(int(ss[1])):
        ens.add(ss[j + 2])
    b.append(ens)
    if type == "H":
        H.append(b)
    else:
        V.append(b)

V.sort(key=lambda x: len(x[1]))  # sorting the vertical photos (according to the length of the tags) to combine
# each two of them in one slide

twoV = []  # will contain the slides that contains two vertical photos


def min_of_three(a, b):  # the function that calculates the special value
    n = len(a.intersection(b))
    return min(n, len(a) - n, len(b) - n)


if len(V) % 2 == 1:  # if the number of the vertical photos we should remove a photo
#del V[len(V) // 2]
    del V[0]
l = len(V)
dernier = V[0]
del V[0]
for i in range(l// 2):  # make each two vertical photos in one slide
    if i%100 == 0 :
        print("partie A",i/(l//2))
    b = []
    imin = 0
    mn = min_of_three(dernier[1] , V[0][1])
    for j in range(1,len(V)) :
        k= min_of_three(dernier[1] , V[j][1])
        if k < mn :
            mn = k
            imin = j



    b.append([V[imin][0], dernier[0]])
    b.append(V[imin][1].union(dernier[1]))
    twoV.append(b)

    del V[imin]

    if len(V) :
        dernier = V[0]
        del V[0]

# combine all the slides in one list then sorting them according to the length of the tags
total = twoV + H
total.sort(key=lambda x: len(x[1]))
#total.reverse()

# for each slide we have to find the slide in the rest of slides which makes more points (starting by the first
# slide)
arr = []
arr.append(total[0])  # adding the first slide to the list
dernier = arr[0]  # Saving the last slide treated
del total[0]  # once added and saved we remove it
l = len(total)

for i in range(l):  # for each slide
    if i%100 == 0 : print("partie B",i)
    mx = min_of_three(total[0][1], dernier[1])  # the first slide after removing the first slide
    imax = 0
    for j in range(1, len(total)):  # for each slide of the rest of slides
        v = min_of_three(total[j][1], dernier[1])  # we calculate the special value
        if v > mx:  # when we find a new value greater than the old one we save it
            mx = v
            imax = j
    arr.append(total[imax])  # then we add the slide
    dernier = total[imax]  # save it
    del total[imax]  # and delete it
# writing the results in new file
total = arr
ff = open(o[ll], 'w')
ff.write(str(len(total)) + '\n')
for i in range(len(total)):
    try:
        ff.write(str(total[i][0][0]) + " " + str(total[i][0][1]))
    except:
        ff.write(str(total[i][0]))
ff.write("\n")
ff.close()
