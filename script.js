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


//flèche
const competence = document.querySelector('.competence');

document.querySelector('.fleche.gauche').onclick = () => {
    competence.scrollBy({ left: -700, behavior: 'smooth' });
};

document.querySelector('.fleche.droit').onclick = () => {
    competence.scrollBy({ left: 700, behavior: 'smooth' });
};




//scrolle
let currentSection = 0;
const sections = document.querySelectorAll('.page');

window.addEventListener('wheel', (e) => {
  if (e.deltaY > 0) {
    currentSection = Math.min(currentSection + 1, sections.length - 1);
  } else {
    currentSection = Math.max(currentSection - 1, 0);
  }
  sections[currentSection].scrollIntoView({ behavior: 'smooth' });
});