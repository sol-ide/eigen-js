
const expect = require( "chai" ).expect
const eigen = require("bindings")("EigenJs");

describe("Basic Matrix Binary Operations", () => {
  it("Test for equality", () => {
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
        1, 1, 1,
        2, 2, 2,
        3, 3, 3
      ])
    );

    expect( eigen.equals(m1, m2).eval() );
    expect( eigen.equals(m1, m3).eval() );
  });

  it("Adds two Matrix", () => {
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

    const result = eigen.add( m1, m2 ).eval();

    const expected = new eigen.Matrix(
      3, 3,
      new Float64Array([
        4, 4, 4,
        4, 4, 4,
        4, 4, 4
      ])
    );

    expect( eigen.equals(result, expected).eval() );
  });
});

