<?php
$servername = "localhost";
$username = "root";
$password = "topslayers12#";
$database = "finger_prints";

// Create a connection
$conn = new mysqli($servername, $username, $password, $database);

// Check the connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Handle incoming data from ESP8266
$data = $_GET['data']; // You can use POST if needed
$sql = "insert into finger_apperances(name) values('$data')";

if ($conn->query($sql) === TRUE) {
    echo "Data inserted successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>

