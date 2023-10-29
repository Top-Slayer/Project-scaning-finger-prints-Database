const http = require('http');
const mysql = require('mysql2');

// Create a connection to your MySQL database
const connection = mysql.createConnection({
    host: '127.0.0.1',
    user: 'root',
    password: 'topslayers12#',
    database: 'finger_prints'
});

// Connect to the MySQL database
connection.connect((err) => {
    if (err) {
        console.error('Error connecting to MySQL:', err);
        return;
    }
    console.log('Connected to MySQL');

    // Execute a SELECT query
    connection.query('SELECT * FROM finger_apperances', (queryErr, results) => {
        if (queryErr) {
            console.error('Error executing query:', queryErr);
            connection.end(); // Close the connection in case of an error
            return;
        }
        console.log('Query results:', results);
    });
});

http.createServer((req, res) => {
    res.write(req)
    res.end()
}).listen(3000, () => {
    console.log("Start server");
});

