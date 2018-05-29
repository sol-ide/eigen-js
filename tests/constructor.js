
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

  it("Test Ones Construction", () => {
    let m1 = eigen.Matrix.Ones( 4, 4 );

    let expectedMatrix = new eigen.Matrix(
      4, 4,
      new Float64Array([
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
      ])
    )

    expect( eigen.equals( m1, expectedMatrix ).eval() ).to.be.true;


    let v1 = eigen.RowVector.Ones( 4 );

    let expectedRowVector = new eigen.RowVector(
      new Float64Array([
        1, 1, 1, 1
      ])
    )

    expect( eigen.equals( v1, expectedRowVector ).eval() ).to.be.true;


    let v2 = eigen.ColumnVector.Ones( 4 );

    let expectedColumnVector = new eigen.ColumnVector(
      new Float64Array([
        1,
        1,
        1,
        1
      ])
    )

    expect( eigen.equals( v2, expectedColumnVector ).eval() ).to.be.true;
  });
});

