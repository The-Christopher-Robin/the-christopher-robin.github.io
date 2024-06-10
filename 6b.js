var express = require('express');
var app = express();
var MongoClient = require('mongodb').MongoClient;

MongoClient.connect("mongodb://127.0.0.1/college", function (err, db) {
    if (!err) {
        console.log("Connected to MongoDB");

        app.use(express.static('public'));

        // Serve 6b.html at the root URL
        app.get('/', function(req, res){
            res.sendFile(__dirname+'/'+'6b.html')
        });

        // app.get('/insert.html', function(req, res){
        //     res.sendFile(__dirname+'/'+'6b.html')
        // });

        app.get('/insert.html', function(req, res){
            res.sendFile(__dirname+'/'+'insert.html');
        });


        app.get('/update.html', function(req, res){
            res.sendFile(__dirname+'/'+'update.html');
        });


        app.get('/delete.html', function(req, res){
            res.sendFile(__dirname+'/'+'delete.html');
        });

        // Insert operation
        app.get('/insert', function (req, res) {
            var id = req.query.id;
            var namee = req.query.namee;
            var title = req.query.title;
            var branch = req.query.branch;

            db.collection('department').insertOne({ id: id, name: namee, title: title, branch: branch }, function (err, result) {
                if (err) {
                    console.log("Error inserting data: " + err);
                    res.send("Error inserting data");
                } else {
                    console.log("Data inserted successfully");
                    res.send("Data inserted successfully");
                }
            });
        });

        // Display operation
        app.get('/display', function (req, res) {
            db.collection('department').find({ branch: 'cse', title: 'professor' }).toArray(function (err, data) {
                if (err) {
                    console.log("Error fetching data: " + err);
                    res.send("Error fetching data");
                } else {
                    console.log("Data retrieved successfully");
                    res.send(JSON.stringify(data));
                }
            });
        });

        // Update operation
        app.get('/update', function (req, res) {
            var idToUpdate = req.query.idToUpdate;
            var newTitle = req.query.newTitle;

            db.collection('department').updateOne({ id: idToUpdate }, { $set: { title: newTitle } }, function (err, result) {
                if (err) {
                    console.log("Error updating data: " + err);
                    res.send("Error updating data");
                } else {
                    console.log("Data updated successfully");
                    res.send("Data updated successfully");
                }
            });
        });

        // Delete operation
        app.get('/delete', function (req, res) {
            var idToDelete = req.query.idToDelete;

            db.collection('department').deleteOne({ id: idToDelete }, function (err, result) {
                if (err) {
                    console.log("Error deleting data: " + err);
                    res.send("Error deleting data");
                } else {
                    console.log("Data deleted successfully");
                    res.send("Data deleted successfully");
                }
            });
        });

        // Start the server
        app.listen(5000, function () {
            console.log("Server started on port 5000");
        });
    } else {
        console.log("Error connecting to MongoDB: " + err);
    }
});
