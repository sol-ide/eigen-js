
var eigen = require("bindings")("EigenJs");

const m1 = new eigen.Matrix(
  3, 3,
  new Float64Array([
    1, 1, 1,
    2, 2, 2,
    3, 3, 3
  ])
);

const m2 = new eigen.Matrix(
  3, 3,
  new Float64Array([
    3, 3, 3,
    2, 2, 2,
    1, 1, 1
  ])
);

const m3 = new eigen.Matrix(
  3, 3,
  new Float64Array([
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
  ])
);

eigen.multiply(
  m1,
  eigen.add( m2, m3 )
).eval().then((m) => {
  console.log( m1.rows );
  console.log( m1.columns );
});

