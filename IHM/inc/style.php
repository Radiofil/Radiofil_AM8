<style>
.panneau_sup {
  border: 2px solid #000000;
  border-radius: 15px;
  display: grid;
  grid-template-columns: 1fr 5fr 2fr;
}

.panneau_gauche {
  align-self: center;
}

.panneau_liste_freq {
  border: 2px solid #000000;
  border-radius: 15px;
  margin-right: 10px;
}

.panneau_inf {
  border: 2px solid #000000;
  border-radius: 15px;
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
}

.clavier {
  align-self: center;
    display: grid;
  grid-template-columns: 1fr 3fr 1fr;
}

ul {
  list-style: none;
}

ul lir::before {
  content: "\2022";
  color: red;
  font-weight: bold;
  display: inline-block; 
  width: 1em;
  margin-left: -1em;
}

ul liv::before {
  content: "\2022";
  color: green;
  font-weight: bold;
  display: inline-block; 
  width: 1em;
  margin-left: -1em;
}

ul lij::before {
  content: "\2022";
  color: yellow;
  font-weight: bold;
  display: inline-block; 
  width: 1em;
  margin-left: -1em;
}

ul li::before {
  content: "\2022";
  color: black;
  font-weight: bold;
  display: inline-block; 
  width: 1em;
  margin-left: -1em;
}

.bouton {
margin: 2px 2px;
padding: 25px 10px;
 border-radius: 12px;
}

/* The Modal (background) */
.modal {
  display: none; /* Hidden by default */
  position: fixed; /* Stay in place */
  z-index: 1; /* Sit on top */
  padding-top: 200px; /* Location of the box */
  left: 0;
  top: 0;
  width: 100%; /* Full width */
  height: 100%; /* Full height */
  overflow: auto; /* Enable scroll if needed */
  background-color: rgb(0,0,0); /* Fallback color */
  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
}

/* Modal Content */
.modal-content {
  background-color: #fefefe;
text-align: left;
  margin: auto;
  padding: 20px;
  border: 1px solid #888;
  width: 30%;
}

/* The Close Button */
.close {
  color: #aaaaaa;
  float: right;
  font-size: 28px;
  font-weight: bold;
}

.close:hover,
.close:focus {
  color: #000;
  text-decoration: none;
  cursor: pointer;
}
</style>
