let monElement;

// Script pour charger le header et changement de theme pour les page php
fetch('header.html')
    .then(response => response.text())
    .then(data => {
        document.getElementById('header').innerHTML = data;
        // Accéder à l'élément après le chargement
        monElement = document.getElementById('theme-toggle');
        if (monElement) {
            console.log('Élément trouvé:', monElement);
            // Ajouter l'écouteur d'événements ici
            monElement.addEventListener('change', function() {
                let themeLink = document.getElementById('theme-link');
                if (this.checked) {
                    themeLink.setAttribute('href', 'themeSombre.css');
                } else {
                    themeLink.setAttribute('href', 'themeClaire.css');
                }
            });
        } else {
            console.log('Élément non trouvé');
        }
    })
    .catch(error => console.error('Erreur:', error));

//script affichage des projet et des documment
document.addEventListener('DOMContentLoaded', function () {
    const tabs = document.querySelectorAll('.tab');
    const contents = document.querySelectorAll('.tab-content');

    tabs.forEach(tab => {
        tab.addEventListener('click', function () {
            tabs.forEach(t => t.classList.remove('active'));
            contents.forEach(c => c.classList.remove('active'));

            tab.classList.add('active');
            document.getElementById(tab.getAttribute('data-tab')).classList.add('active');
        });
    });
});


// Fonction pour charger le footer
fetch("footer.html")
    .then(response => response.text())
    .then(data => {
        document.getElementById("footer").innerHTML = data;
    })
    .catch(error => console.error("Erreur lors du chargement du fichier :", error));
