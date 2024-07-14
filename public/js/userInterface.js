import { setHasPath } from './mazeLogic.js';
import {
	generatorSelect,
	generatorSpeedSelect,
	generateButton,
	solverSelect,
	solverSpeedSelect,
	solveButton,
	clearButton,
} from './controller.js';

let width = 0;
let height = 0;

export function getWidth() {
	return width;
}

export function getHeight() {
	return height;
}

export function showToast(type, title, message) {
	const toast = document.getElementById('toast');
	const toastIcon = toast.querySelector('.toastIcon');
	const toastTitle = toast.querySelector('.toastTitle');
	const toastMessage = toast.querySelector('.toastMessage');

	toast.className = '';
	toast.className = `toast${title}`;

	const icons = {
		success: './assets/Icons/success.svg',
		info: './assets/Icons/info.svg',
		warning: './assets/Icons/warning.svg',
		error: './assets/Icons/error.svg',
	};

	const iconURL = icons[type];

	if (iconURL) {
		toastIcon.data = iconURL;
	}

	toastTitle.textContent = title;
	toastMessage.textContent = message;
	toast.classList.add('show');

	setTimeout(() => {
		toast.classList.remove('show');
	}, 3000);
}

export function updateCellClass(y, x, isWithAnimation) {
	const cell = document.getElementById(`cell-${y}-${x}`);
	const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

	if (cell) {
		cell.className = 'cell';

		if (cellType === -1) {
			if (isWithAnimation === 1 && cell.tagName !== 'DIV') {
				cell.classList.add('wallAnimation');
			}

			cell.classList.add('wall');
		} else if (cellType === 3) {
			cell.classList.add('entry');
		} else if (cellType === 4) {
			cell.classList.add('exit');
		} else if (cellType === 2147483646) {
			cell.classList.add('wallAnimation', 'path');
			setHasPath(true);
		}
	}
}

export function initializeMazeUI(mazeContainer) {
	Module.onRuntimeInitialized = () => {
		const cellSize = 30;
		const mazeRect = mazeContainer.getBoundingClientRect();
		const tempWidth = Math.floor(mazeRect.width / cellSize);
		const temHeight = Math.floor(mazeRect.height / cellSize);

		width = tempWidth % 2 === 0 ? tempWidth - 1 : tempWidth;
		height = temHeight % 2 === 0 ? temHeight - 1 : temHeight;

		mazeContainer.style.gridTemplateRows = `repeat(${height}, ${cellSize}px)`;
		mazeContainer.style.gridTemplateColumns = `repeat(${width}, ${cellSize}px)`;
		mazeContainer.innerHTML = '';

		Module.ccall('initializeMaze', null, ['number', 'number'], [height, width]);
		Module.ccall('setEntryExit', null, null, null);

		for (let y = 0; y < height; y++) {
			for (let x = 0; x < width; x++) {
				const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);
				const cell = document.createElement(
					x === 0 || x === width - 1 || y === 0 || y === height - 1 ? 'div' : 'button'
				);

				cell.id = `cell-${y}-${x}`;
				if (cell.tagName === 'BUTTON') {
					cell.tabIndex = -1;
				}

				mazeContainer.appendChild(cell);
				updateCellClass(y, x, 0);
			}
		}
	};
}

export function setControlsState(isEnabled) {
	generatorSelect.disabled = !isEnabled;
	generatorSpeedSelect.disabled = !isEnabled;
	solverSelect.disabled = !isEnabled;
	solverSpeedSelect.disabled = !isEnabled;
	solveButton.disabled = !isEnabled;
	clearButton.disabled = !isEnabled;

	if (generatorSelect.selectedIndex !== 6) {
		generateButton.disabled = !isEnabled;
	}

	document.body.classList.toggle('not-allowed', !isEnabled);
}

export function closeMenu() {
	const aside = document.querySelector('aside');
	aside.classList.remove('open');
	document.getElementById('openHambMenu').style.display = 'flex';
}

document.addEventListener('DOMContentLoaded', () => {
	const numParticles = 40;
	const particleDiv = document.getElementById('particles');

	// Creation of particles for the background of the page
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

	// Hamburger menu
	const openButton = document.getElementById('openHambMenu');
	const closeButton = document.getElementById('closeHambMenu');
	const aside = document.querySelector('aside');

	openButton.addEventListener('click', () => {
		aside.classList.toggle('open');
		openButton.style.display = 'none';
	});

	closeButton.addEventListener('click', () => {
		closeMenu();
	});
});
