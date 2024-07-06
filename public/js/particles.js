document.addEventListener('DOMContentLoaded', () => {
    const numParticles = 40;
    const particleDiv = document.getElementById('particles');

    for (let i = 0; i < numParticles; i++) {
        const particle = document.createElement('div');
        const size = Math.random() * 4 + 1 + 'px';

        particle.className = 'particle';

        particle.style.left = Math.random() * 100 + 'vw';
        particle.style.top = Math.random() * 100 + 'vh';
        particle.style.width = size;
        particle.style.height = size;

        particleDiv.appendChild(particle);
    }
});
