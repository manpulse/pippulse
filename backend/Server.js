const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");

const app = express();
app.use(cors());
app.use(express.json());

app.get("/", (req, res) => {
  res.send("API is running 🚀");
});

mongoose.connect("mongodb://mongo:27017/mernDB");

app.listen(5000, () => console.log("Server running on port 5000"));