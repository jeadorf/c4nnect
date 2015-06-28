// ----------------------------------------------------------------------------
// Constants
// ----------------------------------------------------------------------------

var NUM_ROWS = 6;
var NUM_COLS = 7;
var NONE = 'None';
var AI = 2;

// ----------------------------------------------------------------------------
// Board
// ----------------------------------------------------------------------------

function Board() {
  this.pieces = [
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0],
  ];
  this.code = '0';
  this.winner = 0;
}

Board.prototype.get = function(row, col) {
  return this.pieces[col][row];
}

Board.prototype.hasWinner = function() {
  return this.winner != 0;
}

Board.prototype.hasAiWon = function() {
  return this.winner == AI;
}

Board.prototype.decode = function(json) {
  this.pieces = json.pieces;
  this.code = json.code;
  this.winner = json.winner;
}

// ----------------------------------------------------------------------------
// Renderer
// ----------------------------------------------------------------------------

function Renderer(canvas) {
  this.canvas = canvas;
  this.canvas.width = 350;
  this.canvas.height = 300;
}

Renderer.prototype.render = function(board) {
  var ctx = this.canvas.getContext('2d');
  ctx.save();
  ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
  ctx.translate(0, 300);
  ctx.scale(1, -1);
  this.drawRows(ctx, board);
  this.drawColumns(ctx, board);
  this.drawPieces(ctx, board);
  ctx.restore();
}

Renderer.prototype.drawRows = function(ctx, board) {
  this.drawGrid(ctx, NUM_ROWS + 1, NUM_COLS);
}

Renderer.prototype.drawColumns = function(ctx, board) {
  ctx.save();
  ctx.rotate(Math.PI / 2);
  ctx.scale(1, -1);
  this.drawGrid(ctx, NUM_COLS + 1, NUM_ROWS);
  ctx.restore();
}

Renderer.prototype.drawGrid = function(ctx, count, length) {
  ctx.save();
  for (var i = 0; i < count; i++) {
    ctx.beginPath();
    ctx.moveTo(0, 0);
    ctx.lineTo(length * 50, 0);
    ctx.stroke();
    ctx.translate(0, 50);
  }
  ctx.restore();
}

Renderer.prototype.drawPieces = function(ctx, board) {
  for (var c = 0; c < NUM_COLS; c++) {
    for (var r = 0; r < NUM_ROWS; r++) {
      var p = board.get(r, c);
      if (p != 0) {
        this.drawPiece(ctx, r, c, p);
      } else if (c == this.nextColumn) {
        this.drawPiece(ctx, r, c, p);
        break;
      } else {
        break;
      }
    }
  }
}

Renderer.prototype.drawPiece = function(ctx, row, col, player) {
  ctx.save();
  ctx.translate(50 * col, 50 * row);
  ctx.fillStyle = (player == 1 ? 'red' : 'blue');
  ctx.beginPath();
  ctx.arc(25, 25, 20, 0, 2 * Math.PI);
  ctx.fill();
  ctx.restore();
}

// ----------------------------------------------------------------------------
// Engine
// ----------------------------------------------------------------------------

var renderer;
var currentBoard;
var nextBoards;
var inflight;

function fetchBoard(code, ai) {
  inflight = true;
  $.get('/c4nnect/web/server.py',
    {
      command: ai ? 'move' : 'get',
      board: '' + code
    })
  .done(onFetchBoardSuccess)
  .fail(onFetchBoardFailure)
  .always(function() {
    inflight = false;
  });
}

function onFetchBoardSuccess(json) {
  currentBoard = new Board();
  currentBoard.decode(json.current);
  renderer.render(currentBoard);
  nextBoards = $.map(json.next, function(js) {
    if (js) {
      var n = new Board();
      n.decode(js);
      return n;
    } else {
      return NONE;
    }
  });
  if (currentBoard.hasWinner()) {
    if (currentBoard.hasAiWon()) {
      alert('Ouch, you lost!');
    } else {
      alert('Congratulations, you won!');
    }
  }
}

function onFetchBoardFailure() {
  alert('Error, failed to get board from server.');
}

function canPutPiece(col) {
  return nextBoards[col] != NONE;
}

function putPiece(col) {
  if (!inflight && canPutPiece(col)) {
    renderer.render(nextBoards[col]);
    fetchBoard(nextBoards[col].code, true);
  }
}

function onBoardClick(evt) {
  var col = Math.floor(evt.offsetX / 50);
  if (col >= 0 && col < NUM_COLS) {
    putPiece(col);
  }
}

function newGame() {
  if (!inflight) {
    fetchBoard(0, false);
  }
}

function onLoad() {
  var canvas = document.getElementById('board');
  renderer = new Renderer(canvas);
  newGame();
  canvas.addEventListener("click", onBoardClick, false);
}

