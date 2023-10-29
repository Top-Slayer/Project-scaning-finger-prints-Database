var admin = require("firebase-admin");

var serviceAccount = require("realtime-apartment-firebase-adminsdk-n6y4e-e9532ed571.json");

admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    // databaseURL: "https://realtime-apartment-default-rtdb.asia-southeast1.firebasedatabase.app"
});

const collectionRef = db.collection("1");

collectionRef.get()
    .then((snapshot) => {
        snapshot.forEach((doc) => {
            console.log("Document data:", doc.data());
        });
    })
    .catch((error) => {
        console.error("Error getting documents: ", error);
    });
