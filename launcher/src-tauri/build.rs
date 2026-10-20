fn main() {
  // Compila o core mgba ESTATICAMENTE dentro do .exe - sem .dll externa
  // Para o jogador final é 1 exe só, igual bbport pkg
  let mut build = cc::Build::new();
  build.include("../../core/mgba/include");
  build.include("../../core/mgba/src");
  build.define("M_CORE_GBA", None);
  build.define("STATIC_LINK", None);
  build.file("../../core/mgba/src/gba/core.c");
  build.file("../../core/mgba/src/gba/video.c");
  build.file("../../core/mgba/src/core/core.c");
  build.static_flag(true);
  build.compile("mgba_static");
  tauri_build::build()
}
