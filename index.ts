import assert from "assert";

/**
 * E  -> TE'
 * E' -> +TE' | -TE' | e
 * T  -> FT'
 * T' -> *FT' | /FT' | e
 * F  -> N | (E) | sin(E) | cos(E)
 * N  -> DN'
 * N' -> DN' |  e
 * D  -> 0 | ... | 9
 */
class IllegalInput extends Error {}

class Parser {
  input: string = "";
  position: number = 0;

  Parse(s: string) {
    this.input = s + "$";
    this.position = 0;
    this.E();
    this.Match("$");
  }

  private LookAhead(length: number = 1) {
    return this.input.slice(this.position, this.position + length);
  }

  private Match(token: string) {
    if (this.LookAhead(token.length) === token) {
      this.position += token.length;
    } else {
      throw new IllegalInput();
    }
  }

  private E() {
    if (
      this.LookAhead() === "(" ||
      ["sin(", "cos("].includes(this.LookAhead(4)) ||
      (parseInt(this.LookAhead()) >= 0 && parseInt(this.LookAhead()) <= 9)
    ) {
      console.log(`E -> TE'\n`);
      this.T();
      this.E_();
    } else {
      throw new IllegalInput();
    }
  }

  private E_() {
    if (["+", "-"].includes(this.LookAhead())) {
      console.log(`E' -> ${this.LookAhead()}TE'\n`);
      this.Match(this.LookAhead());
      this.T();
      this.E_();
    } else if ([")", "$"].includes(this.LookAhead())) {
      console.log(`E' -> e\n`);
    } else {
      throw new IllegalInput();
    }
  }

  private T() {
    if (
      this.LookAhead() === "(" ||
      ["sin(", "cos("].includes(this.LookAhead(4)) ||
      (parseInt(this.LookAhead()) >= 0 && parseInt(this.LookAhead()) <= 9)
    ) {
      console.log(`T -> FT'\n`);
      this.F();
      this.T_();
    } else {
      throw new IllegalInput();
    }
  }

  private T_() {
    if (["*", "/"].includes(this.LookAhead())) {
      console.log(`T' -> ${this.LookAhead()}FT'\n`);
      this.Match(this.LookAhead());
      this.F();
      this.T_();
    } else if (["+", "-", ")", "$"].includes(this.LookAhead())) {
      console.log(`T' -> e\n`);
    } else {
      throw new IllegalInput();
    }
  }

  private F() {
    if (parseInt(this.LookAhead()) >= 0 && parseInt(this.LookAhead()) <= 9) {
      console.log(`F -> N\n`);
      this.N();
    } else if (this.LookAhead() === "(") {
      this.Match("(");
      this.E();
      this.Match(")");
      console.log(`F -> (E)\n`);
    } else if (["sin(", "cos("].includes(this.LookAhead(4))) {
      this.Match(this.LookAhead(4));
      this.E();
      this.Match(")");
      console.log(`F -> ${this.LookAhead(4)}E)\n`);
    } else {
      throw new IllegalInput();
    }
  }

  private N() {
    if (parseInt(this.LookAhead()) >= 0 && parseInt(this.LookAhead()) <= 9) {
      console.log(`N' -> DN'\n`);
      this.D();
      this.N_();
    } else {
      throw new IllegalInput();
    }
  }

  private N_() {
    if (parseInt(this.LookAhead()) >= 0 && parseInt(this.LookAhead()) <= 9) {
      console.log(`N' -> DN'\n`);
      this.D();
      this.N_();
    } else if (["+", "-", "*", "/", ")", "$"].includes(this.LookAhead())) {
      console.log(`N' -> e\n`);
    } else {
      throw new IllegalInput();
    }
  }

  private D() {
    if (parseInt(this.LookAhead()) >= 0 && parseInt(this.LookAhead()) <= 9) {
      console.log(`D -> ${this.LookAhead()}\n`);
      this.Match(this.LookAhead());
    } else {
      throw new IllegalInput();
    }
  }
}

const checkInput = (s: string) => {
  const parser = new Parser();
  try {
    parser.Parse(s);
    console.log(`Accepted: ${s}\n`);
    return true;
  } catch (error) {
    console.log(`Not accepted: ${s}\n`);
    return false;
  }
};

// OK
assert.strictEqual(checkInput("12+3*5"), true);
assert.strictEqual(checkInput("sin(30)*(10+cos(3*3))"), true);
assert.strictEqual(checkInput("sin(30+cos(60)*4)+4*5"), true);

// NOK OK
assert.strictEqual(checkInput("12+3*sin(90"), false);
assert.strictEqual(checkInput("sin(30)*(10+cos(3*3)))"), false);
assert.strictEqual(checkInput("sin(30cos(60))"), false);
