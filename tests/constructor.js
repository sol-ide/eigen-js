
const expect = require( "chai" ).expect
const eigen = require("bindings")("EigenJs");

describe("Eigen Constructors", () => {
  it("Test Matrix Construction", () => {
    let m = new eigen.Matrix(
      3, 3,
      new Float64Array([
        1, 1, 1,
        2, 2, 2,
        3, 3, 3
      ])
    );
  });

  it("Test Vector Construction", () => {
    let v1 = new eigen.RowVector(
      new Float64Array([
        1, 2, 3, 4, 5, 6
      ])
    );

    let v2 = new eigen.ColumnVector(
      new Float64Array([
        1,
        2,
        3,
        4,
        5,
        6
      ])
    );
  });
});

