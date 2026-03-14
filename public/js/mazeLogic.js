import { CellType } from './constants.js';
import { getGeneratorSpeedInterval, getSolverSpeedInterval, getGeneratorAlgo } from './controller.js';
import { showToast, updateCellClass, setControlsState, getWidth, getHeight } from './userInterface.js';

const state = {
	updateTimer: null,
	isGenerating: false,
	isSolving: false,
	hasPath: false,
};

export function getUpdateTimer()           { return state.updateTimer; }
export function setUpdateTimer(timer)      { state.updateTimer = timer; }
export function getIsGenerating()          { return state.isGenerating; }
export function setIsGenerating(value)     { state.isGenerating = value; }
export function getIsSolving()             { return state.isSolving; }
export function setIsSolving(value)        { state.isSolving = value; }
export function getHasPath()               { return state.hasPath; }
export function setHasPath(value)          { state.hasPath = value; }

export function prepareMaze() {
	const algo = Number(getGeneratorAlgo());

	let prepareType;
	if (algo === 1 || algo === 3 || algo === 5 || algo === 6) {
		prepareType = 3;
	} else if (algo === 2) {
		prepareType = 2;
	} else {
		prepareType = 1;
	}

	Module.ccall('prepareMaze', null, ['number'], [prepareType]);

	for (let y = 1; y < getHeight() - 1; y++) {
		for (let x = 1; x < getWidth() - 1; x++) {
			updateCellClass(y, x, 0);
		}
	}
}

export function restorePath() {
	for (let y = 1; y < getHeight(); y++) {
		for (let x = 1; x < getWidth(); x++) {
			const cellType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, x]);

			if (cellType === CellType.PATH) {
				Module.ccall('setTypeCell', null, ['number', 'number', 'number'], [y, x, CellType.EMPTY]);
				updateCellClass(y, x, 0);
			}
		}
	}
}

function getCellOrder() {
	const size = Module.ccall('getCellOrderSize', 'number', [], []);
	const cellOrder = [];

	for (let i = 0; i < size; i++) {
		const ptrY = Module._malloc(4);
		const ptrX = Module._malloc(4);

		Module.ccall('getCellOrder', null, ['number', 'number', 'number'], [i, ptrY, ptrX]);

		cellOrder.push({
			y: Module.HEAP32[ptrY >> 2],
			x: Module.HEAP32[ptrX >> 2],
		});

		Module._free(ptrY);
		Module._free(ptrX);
	}

	return cellOrder;
}

function onAnimationEnd(isGeneration) {
	if (isGeneration) {
		setIsGenerating(false);
	} else {
		setIsSolving(false);

		if (getHasPath()) {
			showToast('success', 'Success', 'Maze solved successfully! Path to the exit is available.');
		} else {
			showToast('error', 'Error', 'Maze solution not found. No path exists.');
		}
	}

	setControlsState(true);
	setHasPath(false);
}

export function updateMaze(isGeneration) {
	const interval = isGeneration ? getGeneratorSpeedInterval() : getSolverSpeedInterval();
	const cellOrder = getCellOrder();
	let index = 0;

	// Show entry and exit before animating the rest
	for (let y = 1; y < getHeight() - 1; y++) {
		const entryType = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, 1]);
		const exitType  = Module.ccall('getTypeCell', 'number', ['number', 'number'], [y, getWidth() - 2]);

		if (entryType === CellType.ENTRY) updateCellClass(y, 1, 0);
		if (exitType  === CellType.EXIT)  updateCellClass(y, getWidth() - 2, 0);
	}

	if (interval === 0) {
		cellOrder.forEach(({ y, x }) => updateCellClass(y, x, 0));
		onAnimationEnd(isGeneration);
		return;
	}

	setUpdateTimer(
		setInterval(() => {
			if (index >= cellOrder.length) {
				clearInterval(getUpdateTimer());
				setUpdateTimer(null);
				onAnimationEnd(isGeneration);
				return;
			}

			const { y, x } = cellOrder[index];
			updateCellClass(y, x, 1);
			index++;
		}, interval)
	);

	if (isGeneration) {
		setIsGenerating(true);
	} else {
		setIsSolving(true);
	}

	setControlsState(false);
}
