
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

    expect( eigen.equals(m1, m2).eval() ).to.be.false;
    expect( eigen.equals(m1, m3).eval() ).to.be.true;
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

    const result = eigen.add( m1, m2 );

    const expected = new eigen.Matrix(
      3, 3,
      new Float64Array([
        4, 4, 4,
        4, 4, 4,
        4, 4, 4
      ])
    );

    let expression = eigen.equals(result, expected);
    expect( expression.eval() ).to.be.true;

    expression.then((e)=> {
      expect(e).to.be.true;
    }).catch(()=>{
      expect.fail();
    });
  });

  it("Fails to add two Matrix with different size", () => {
    const m1 = new eigen.Matrix(
      3, 3,
      new Float64Array([
        1, 1, 1,
        2, 2, 2,
        3, 3, 3
      ])
    );
    
    const m2 = new eigen.Matrix(
      4, 4,
      new Float64Array([
        4, 4, 4, 4,
        3, 3, 3, 3,
        2, 2, 2, 2,
        1, 1, 1, 1
      ])
    );

    let expression = () => { eigen.add( m1, m2 ); }
    expect( expression ).to.throw();
  });

  it("Matrix Product", () => {
    const m1 = new eigen.Matrix(
      3, 10,
      new Float64Array([
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3
      ])
    );
    
    const m2 = new eigen.Matrix(
      10, 2,
      new Float64Array([
       10,10,
        9, 9,
        8, 8,
        7, 7,
        6, 6,
        5, 5,
        4, 4,
        3, 3,
        2, 2,
        1, 1
      ])
    );

    const result = eigen.product( m1, m2 );

    const expected = new eigen.Matrix(
      3, 2,
      new Float64Array([
        55, 55,
        110, 110,
        165, 165
      ])
    );


    let expression = eigen.equals(result, expected);
    expect( expression.eval() ).to.be.true;

    expression.then((e)=> {
      expect(e).to.be.true;
    }).catch(()=>{
      expect.fail();
    });
  });
});

