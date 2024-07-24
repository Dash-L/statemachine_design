with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "statemachine-design-env";
  buildInputs = [
    gcc
    clang-tools
  ];
}
