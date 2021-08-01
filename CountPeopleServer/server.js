// Library
const express = require('express');
const sqlite3 = require('sqlite3').verbose();

let db = new sqlite3.Database('data.db', sqlite3.OPEN_READWRITE);

const app = express();

app.use(express.urlencoded({ extended: false }));

app.get('/', (req, res) => {
    res.send("Count People Server");
    return res.status(200);
});

app.get('/get', (req, res) => {
    console.log("# GET Request");

    db.all('SELECT * FROM Count_People', function (err, rows, fields) {
        res.setHeader('Content-Type','application/json');
        res.send(JSON.stringify(rows));
    });

    return res.status(201);
});

app.post('/post', (req, res) => {
    console.log("# POST Request");

    db.all('SELECT COUNT(*) as cnt FROM Count_People WHERE locName = ?', req.body.locName, async function (err, rows, fields) {
        // Create
        if (rows[0].cnt == 0)
            db.run('INSERT INTO Count_People VALUES (?, ?, ?, ?)', [req.body.locName, req.body.loc1, req.body.loc2, req.body.count]);
        // Update
        else
            db.run('UPDATE Count_People SET count = ? WHERE locName = ?', [req.body.count, req.body.locName]);
    });

    return res.status(201).send();
});

app.listen(32734, () => {
    console.log('# Count People Server port 32734');
});