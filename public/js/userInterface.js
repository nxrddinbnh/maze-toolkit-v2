import { CellType } from './constants.js';
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

	const icons = {
		success: './assets/icons/success.svg',
		info:    './assets/icons/info.svg',
		warning: './assets/icons/warning.svg',
		error:   './assets/icons/error.svg',
	};

	toast.className = `toast${type.charAt(0).toUpperCase() + type.slice(1)}`;

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

	if (!cell) return;

	cell.className = 'cell';

	if (cellType === CellType.WALL) {
		if (isWithAnimation === 1 && cell.tagName !== 'DIV') {
			cell.classList.add('wallAnimation');
		}
		cell.classList.add('wall');
	} else if (cellType === CellType.ENTRY) {
		cell.classList.add('entry');
	} else if (cellType === CellType.EXIT) {
		cell.classList.add('exit');
	} else if (cellType === CellType.PATH) {
		cell.classList.add('wallAnimation', 'path');
		setHasPath(true);
	}
}

export function initializeMazeUI(mazeContainer) {
	Module.onRuntimeInitialized = () => {
		const cellSize = 30;
		const mazeRect = mazeContainer.getBoundingClientRect();
		const tempWidth = Math.floor(mazeRect.width / cellSize);
		const tempHeight = Math.floor(mazeRect.height / cellSize);

		width  = tempWidth  % 2 === 0 ? tempWidth  - 1 : tempWidth;
		height = tempHeight % 2 === 0 ? tempHeight - 1 : tempHeight;

		mazeContainer.style.gridTemplateRows    = `repeat(${height}, ${cellSize}px)`;
		mazeContainer.style.gridTemplateColumns = `repeat(${width}, ${cellSize}px)`;
		mazeContainer.innerHTML = '';

		Module.ccall('initializeMaze', null, ['number', 'number'], [height, width]);
		Module.ccall('setEntryExit', null, null, null);

		for (let y = 0; y < height; y++) {
			for (let x = 0; x < width; x++) {
				const isBorder = x === 0 || x === width - 1 || y === 0 || y === height - 1;
				const cell = document.createElement(isBorder ? 'div' : 'button');

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
	generatorSelect.disabled      = !isEnabled;
	generatorSpeedSelect.disabled = !isEnabled;
	solverSelect.disabled         = !isEnabled;
	solverSpeedSelect.disabled    = !isEnabled;
	solveButton.disabled          = !isEnabled;
	clearButton.disabled          = !isEnabled;

	generateButton.disabled = !isEnabled || generatorSelect.selectedIndex === 6;

	document.body.classList.toggle('not-allowed', !isEnabled);
}

export function closeMenu() {
	const aside = document.querySelector('aside');
	aside.classList.remove('open');
	document.getElementById('openHambMenu').style.display = '';
}

document.addEventListener('DOMContentLoaded', () => {
	const numParticles = 40;
	const particleDiv = document.getElementById('particles');

	for (let i = 0; i < numParticles; i++) {
		const particle = document.createElement('div');
		const size = `${Math.random() * 4 + 1}px`;

		particle.className    = 'particle';
		particle.style.left   = `${Math.random() * 100}vw`;
		particle.style.top    = `${Math.random() * 100}vh`;
		particle.style.width  = size;
		particle.style.height = size;

		particleDiv.appendChild(particle);
	}

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
