

Pr = input("Enter the problem you want to submit (A,B,C,D,E) : ")
if Pr.upper() in ['A' , 'B' , 'C' , 'D' , 'E'] :

    out = input("Enter the output file : ")

    file = {'A':"inputs/a_example.txt", 'B' : "inputs/b_lovely_landscapes.txt",'C':"inputs/c_memorable_moments.txt","D":"inputs/d_pet_pictures.txt",
            "E":"inputs/e_shiny_selfies.txt"}

    f = open(file[Pr.upper()], "r")

    s = f.read()
    s = s.split("\n")
    n = int(s[0])
    f.close()
    arr = []


    for i in range(n):
        ss = s[i + 1].split()
        b = []
        type = ss[0]
        ens = set()
        b.append(i)
        b.append(type)
        for j in range(int(ss[1])):
            ens.add(ss[j + 2])
        b.append(ens)
        b.append(-1)
        arr.append(b)


    def min_of_three(a, b):
        n = len(a.intersection(b))
        return min(n, len(a) - n, len(b) - n)
    stop = False
    try:
        f= open("outputs/"+out , 'r')

        s = f.read().split("\n")
        slides = []
        for i in range(1,int(s[0])+1):
            if s[i].find(" ") == -1 :
                slides.append([int (s[i])])
            else :
                slides.append([int(s[i].split()[0]) ,int(s[i].split()[1])])

        curr = set()
        for j in slides[0]:
            curr = curr.union(arr[j][2])

            if arr[j][3] != -1:
                print("the picture {} is used in line {}".format(j,arr[j][3]))
                stop = True
                break
            else:
                arr[j][3] = 1
        score = 0

        for i in range (1,len(slides)) :

            next = set()
            for j in slides[i] :
                next = next.union(arr[j][2])

                if arr[j][3]!=-1 :
                    print("the picture {} is used in line {} and {}".format(j,arr[j][3] , i+1))
                    stop = True
                    break
                else:
                    arr[j][3] = i+1

            score += min_of_three(curr , next)
            curr = next
            if stop : break


        if not stop :
            print("Your score is : {}".format(score))
    except :
        print("File doesn't exist !")
else :
    print("Error")
