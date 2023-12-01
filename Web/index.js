import { initializeApp } from "https://www.gstatic.com/firebasejs/10.6.0/firebase-app.js";
import { getFirestore, collection, getDocs } from "https://www.gstatic.com/firebasejs/10.6.0/firebase-firestore.js";

const firebaseConfig = {
    apiKey: "AIzaSyB7OVD3bVd3MJ8__RHY4kBYGiRzo1wiPDk",
    authDomain: "realtime-apartment.firebaseapp.com",
    databaseURL:
        "https://realtime-apartment-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "realtime-apartment",
    storageBucket: "realtime-apartment.appspot.com",
    messagingSenderId: "759114484251",
    appId: "1:759114484251:web:6c055482a6b5603ee946be",
    measurementId: "G-LRVMEGHH4C",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const db = getFirestore(app);

const nameDisplay = ['name_1', 'name_2', 'name_3', 'name_4', 'name_5', 'name_6'];
const loginDisplay = ['login_1', 'login_2', 'login_3', 'login_4', 'login_5', 'login_6'];
const logoutDisplay = ['logout_1', 'logout_2', 'logout_3', 'logout_4', 'logout_5', 'logout_6'];
const statusCheck = ['--bg-color1', '--bg-color2', '--bg-color3', '--bg-color4', '--bg-color5', '--bg-color6']
let name = [];
let login = [];
let logout = [];
let status = [];

async function getData(db) {
    const fpCol = collection(db, 'FingerPrint');
    const fpSnapshot = await getDocs(fpCol);
    return fpSnapshot;
}

function showData(valueDP, valueFS) {
    valueDP.forEach((element, index) => {
        document.getElementById(element).innerHTML = valueFS[index];
    });
}

function checkingStatus() {
    status.forEach((e, i) => {
        if (e == true) {
            document.documentElement.style.setProperty(statusCheck[i], '#24e60b');
        }
        else if (e != null) {
            document.documentElement.style.setProperty(statusCheck[i], 'red');
        }
    });
}

function refresh() {
    location.reload()
}

const data = await getData(db);
data.forEach(FingerPrint => {
    const obj = FingerPrint.data();
    name.push((obj.Name != undefined) ? obj.Name : (obj.Status != undefined) ? "[ . . . . . ]" : null);
    login.push((obj.LoginDate != undefined) ? obj.LoginDate : null);
    logout.push((obj.LogoutDate != undefined) ? obj.LogoutDate : null);
    status.push((obj.Status != undefined) ? obj.Status : null);

    console.log(obj);
})

showData(nameDisplay, name);
showData(loginDisplay, login);
showData(logoutDisplay, logout);

checkingStatus();

setInterval(refresh, 5000);