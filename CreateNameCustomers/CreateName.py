import firebase_admin
from firebase_admin import credentials, firestore
from colorama import Fore, Back, Style, init

fingerApperances = [
    "fingerApperance_1",
    "fingerApperance_2",
    "fingerApperance_3",
    "fingerApperance_4",
    "fingerApperance_5",
    "fingerApperance_6",
]

cred = credentials.Certificate("privateKey.json")
firebase_admin.initialize_app(cred)
db = firestore.client()


# Choice Functions
def checkEmptyHouse():
    for x in range(len(fingerApperances)):
        checkCustomer = db.collection("FingerPrint").document(str(fingerApperances[x]))
        check = checkCustomer.get()
        if check.exists:
            data = check.to_dict()
            field_value = data.get("Name")
            if field_value != None:
                print(
                    f"{Fore.LIGHTRED_EX} {x+1} -- House-ID [{x+1}] : Own [{field_value}] --{Fore.RESET}"
                )
            else:
                print(
                    f"{Fore.LIGHTGREEN_EX} {x+1} -- House-ID [{x+1}] : Empty --{Fore.RESET}"
                )


def deleteCustomer():
    print(">> Enter ID-Room for Deleting: ", end="")
    roomID = int(input())
    id = db.collection("FingerPrint").document(str(fingerApperances[roomID - 1]))

    if roomID >= 1 and roomID <= 6:
        id.set(None)
        print(f"{Fore.LIGHTGREEN_EX}Delete success{Fore.RESET}")
    else:
        print(f"{Fore.LIGHTYELLOW_EX}Out of number's house{Fore.RESET}")


def createCustomerName():
    checkEmptyHouse()

    inputName = str(input(">> Enter customer's name: "))
    roomID = int(input(">> Enter ID-Room: "))

    customer = db.collection("FingerPrint").document(str(fingerApperances[roomID - 1]))
    check = customer.get()

    if check.exists:
        data = check.to_dict()
        field_value = data.get("Name")
        if field_value == None:
            data = {"Name": inputName}
            customer.update(data)
            print(f"{Fore.LIGHTGREEN_EX}Create cutomer's name was successful{Fore.RESET}")
        else:
            print(f"{Fore.LIGHTYELLOW_EX}Already have customer{Fore.RESET}")

def help():
    print(
        ">> All command:\n"
        + "1. ceh >> Check empty house\n"
        + "2. ccn >> Create customer name\n"
        + "3. dc >> Delete room-ID of customer make that empty\n"
        + "4. exit >> exit out of program"
    )


# Main processing
while 1:
    command = input(str("# Command >> "))

    if command == "ceh":
        checkEmptyHouse()
    elif command == "ccn":
        createCustomerName()
    elif command == "dc":
        deleteCustomer()
    elif command == "help":
        help()
    elif command == "exit":
        break
    else:
        print('>> Not found command try to type "help" ')

    print("-" * 60)

print(f"{Fore.YELLOW}Exiting processing...{Fore.RESET}")
