import { setMouseEvents } from './cellEvent.js';
import {
	initializeMazeUI,
	setControlsState,
	updateCellClass,
	getHeight,
	getWidth,
	showToast,
} from './userInterface.js';
import {
	prepareMaze,
	restorePath,
	updateMaze,
	getUpdateTimer,
	setUpdateTimer,
	setIsGenerating,
	setIsSolving,
	setHasPath,
	getIsGenerating,
} from './mazeLogic.js';

export const mazeContainer = document.querySelector('#maze');
export const generatorSelect = document.querySelector('#generators');
export const generatorSpeedSelect = document.querySelector('#speedGenerator');
export const solverSelect = document.querySelector('#solvers');
export const solverSpeedSelect = document.querySelector('#speedSolver');
const generateButton = document.querySelector('#generateButton');
const solveButton = document.querySelector('#solveButton');
const clearButton = document.querySelector('#clearButton');
const stopButton = document.querySelector('#stopButton');

function onResize() {
	location.reload();
}

export function getGeneratorAlgo() {
	return generatorSelect.value;
}

export function getSolverAlgo() {
	return solverSelect.value;
}

export function getGeneratorSpeedInterval() {
	switch (generatorSpeedSelect.value) {
		case '1':
			return 0;
		case '2':
			return 5;
		case '3':
			return 10;
		case '4':
			return 20;
		case '5':
			return 40;
		case '6':
			return 200;
		default:
			return 5;
	}
}

export function getSolverSpeedInterval() {
	switch (solverSpeedSelect.value) {
		case '1':
			return 0;
		case '2':
			return 5;
		case '3':
			return 10;
		case '4':
			return 20;
		case '5':
			return 40;
		case '6':
			return 200;
		default:
			return 5;
	}
}

document.addEventListener('DOMContentLoaded', () => {
	generatorSpeedSelect.selectedIndex = 1;
	solverSpeedSelect.selectedIndex = 1;

	initializeMazeUI(mazeContainer);
	window.addEventListener('resize', onResize);
	setMouseEvents(mazeContainer, generatorSelect);

	generateButton.addEventListener('click', () => {
		prepareMaze();
		Module.ccall('generateMaze', null, ['number'], [getGeneratorAlgo()]);
		Module.ccall('setEntryExit', null, null, null);
		updateMaze(1);
	});

	solveButton.addEventListener('click', () => {
		restorePath();
		Module.ccall('solveMaze', null, ['number'], [getSolverAlgo()]);
		updateMaze(0);
	});

	clearButton.addEventListener('click', () => {
		Module.ccall('prepareMaze', null, ['number'], [1]);
		Module.ccall('setEntryExit', null, null, null);

		for (let y = 1; y < getHeight() - 1; y++) {
			for (let x = 1; x < getWidth() - 1; x++) {
				updateCellClass(y, x, 0);
			}
		}
	});

	stopButton.addEventListener('click', () => {
		if (getUpdateTimer()) {
			clearInterval(getUpdateTimer());
			setUpdateTimer(null);
			setIsSolving(false);
			setControlsState(true);
			setHasPath(false);
		}

		if (getIsGenerating()) {
			for (let y = 1; y < getHeight() - 1; y++) {
				for (let x = 1; x < getWidth() - 1; x++) {
					const cell = document.getElementById(`cell-${y}-${x}`);

					if (cell) {
						if (cell.classList.contains('wall')) {
							Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, -1]);
						} else if (!cell.classList.contains('entry') && !cell.classList.contains('exit')) {
							Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, 0]);
						}
					}
				}
			}

			setIsGenerating(false);
		}
	});

	generatorSelect.addEventListener('change', () => {
		if (generatorSelect.selectedIndex === 6) {
			generateButton.disabled = true;
			showToast('info', 'Info', 'You are now in custom mode! Feel free to modify the maze.');
		} else {
			generateButton.disabled = false;
		}
	});
});
