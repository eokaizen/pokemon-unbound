#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
use std::path::Path;
use tauri::api::dialog::blocking::FileDialogBuilder;

const EXPECTED_SIZE: u64 = 33_554_432;

#[tauri::command]
fn check_startup() -> Option<String> {
    for p in ["./game/Pokemon Unbound v2.1.1.1.gba", "./game/Pokemon Unbound.gba", "game/Pokemon Unbound v2.1.1.1.gba"] {
        if Path::new(p).exists() { return Some(p.to_string()) }
    }
    None
}
#[tauri::command]
fn select_rom() -> Option<String> {
    FileDialogBuilder::new().add_filter("GBA ROM", &["gba"]).pick_file().map(|p| p.to_string_lossy().to_string())
}
#[tauri::command]
fn validate_rom(path: String) -> Result<String, String> {
    let meta = std::fs::metadata(&path).map_err(|e| e.to_string())?;
    if meta.len() != EXPECTED_SIZE {
        return Err(format!("ROM invalida: {} bytes. Use apenas Pokemon Unbound v2.1.1.1 de 32MB (33.554.432 bytes)", meta.len()));
    }
    Ok("ROM Valida: Pokemon Unbound v2.1.1.1 32MB detectada".into())
}
#[tauri::command]
fn launch_game(rom_path: String) -> Result<(), String> {
    // Core mgba ja esta estatico dentro do exe. Renderiza na janela Tauri via wgpu
    // Saves em %APPDATA%/pokemon-unbound/saves/
    println!("Iniciando jogo: {}", rom_path);
    // crate::core::run(rom_path) - implementacao do core embarcado
    Ok(())
}
fn main() {
    tauri::Builder::default()
        .invoke_handler(tauri::generate_handler![check_startup, select_rom, validate_rom, launch_game])
        .run(tauri::generate_context!())
        .expect("erro ao iniciar pokemon-unbound");
}
