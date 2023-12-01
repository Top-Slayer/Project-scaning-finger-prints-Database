var admin = require("firebase-admin");

var serviceAccount = require("./privateKey.json");

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://realtime-apartment-default-rtdb.asia-southeast1.firebasedatabase.app"
});

const firestore = admin.firestore();

const docRef = firestore.collection('FingerPrint').doc('fingerApperance_1');

docRef.onSnapshot((doc) => {
    if (doc.exists) {
        location.reload();
    }
});