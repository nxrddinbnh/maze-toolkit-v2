import { CellType, SPEED_INTERVALS } from './constants.js';
import { setMouseEvents } from './cellEvent.js';
import {
	initializeMazeUI,
	setControlsState,
	updateCellClass,
	getHeight,
	getWidth,
	showToast,
	closeMenu,
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

export const mazeContainer         = document.querySelector('#maze');
export const generatorSelect       = document.querySelector('#generators');
export const generatorSpeedSelect  = document.querySelector('#speedGenerator');
export const solverSelect          = document.querySelector('#solvers');
export const solverSpeedSelect     = document.querySelector('#speedSolver');
export const generateButton        = document.querySelector('#generateButton');
export const solveButton           = document.querySelector('#solveButton');
export const clearButton           = document.querySelector('#clearButton');
const stopButton                   = document.querySelector('#stopButton');

export function getGeneratorAlgo() {
	return generatorSelect.value;
}

export function getSolverAlgo() {
	return solverSelect.value;
}

export function getGeneratorSpeedInterval() {
	return SPEED_INTERVALS[generatorSpeedSelect.selectedIndex] ?? SPEED_INTERVALS[1];
}

export function getSolverSpeedInterval() {
	return SPEED_INTERVALS[solverSpeedSelect.selectedIndex] ?? SPEED_INTERVALS[1];
}

document.addEventListener('DOMContentLoaded', () => {
	generatorSpeedSelect.selectedIndex = 1;
	solverSpeedSelect.selectedIndex = 1;
	generateButton.disabled = generatorSelect.selectedIndex === 6;

	generateButton.addEventListener('click', () => {
		prepareMaze();
		Module.ccall('generateMaze', null, ['number'], [getGeneratorAlgo()]);
		Module.ccall('setEntryExit', null, null, null);
		closeMenu();
		updateMaze(1);
	});

	solveButton.addEventListener('click', () => {
		restorePath();
		Module.ccall('solveMaze', null, ['number'], [getSolverAlgo()]);
		closeMenu();
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

		closeMenu();
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
					if (!cell) continue;

					if (cell.classList.contains('wall')) {
						Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, CellType.WALL]);
					} else if (!cell.classList.contains('entry') && !cell.classList.contains('exit')) {
						Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, CellType.EMPTY]);
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

	initializeMazeUI(mazeContainer);
	window.addEventListener('resize', () => location.reload());
	setMouseEvents(mazeContainer, generatorSelect);

	// Ensure cells were created (WASM may load asynchronously)
	setTimeout(() => {
		if (mazeContainer.children.length === 0) {
			location.reload();
		}
	}, 100);
});
