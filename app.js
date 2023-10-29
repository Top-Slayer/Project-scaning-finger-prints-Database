const express = require('express');
const app = express();
const port = 3000; // Choose the port for your server

app.use(express.json()); // Parse JSON request bodies

app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});

app.post('/insertData', (req, res) => {
  // Implement the logic to insert data into your database here
  // You can access the data from the request body using req.body
  const data = req.body;

  // Example response
  const response = {
    message: 'Data inserted successfully',
    data: data,
  };

  res.json(response);
});

