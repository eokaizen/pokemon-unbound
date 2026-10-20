import { invoke } from "@tauri-apps/api/tauri"
import { useEffect, useState } from "react"

export default function App(){
  const [rom, setRom] = useState<string | null>(null)
  const [status, setStatus] = useState("Procurando ROM em /game/...")
  
  useEffect(()=>{ 
    invoke<string | null>("check_startup").then(p=>{
      if(p){ setRom(p); setStatus("✅ ROM encontrada: " + p) }
      else setStatus("Nenhuma ROM em /game/. Clique em Selecionar ROM")
    }) 
  },[])

  async function handleSelect(){
    const path = await invoke<string | null>("select_rom")
    if(!path) return
    setRom(path)
    try{
      const msg = await invoke<string>("validate_rom", { path })
      setStatus("✅ " + msg)
    } catch(e:any){ setStatus("❌ " + e); setRom(null) }
  }
  async function handlePlay(){
    if(!rom) return
    setStatus("Iniciando...")
    await invoke("launch_game", { romPath: rom })
  }
  return (
    <div style={{padding:32, fontFamily:"Segoe UI", maxWidth:700}}>
      <h1>pokemon-unbound</h1>
      <p>Pokémon Unbound v2.1.1.1 - Launcher Nativo</p>
      <div style={{background:"#eee", padding:16, borderRadius:8, margin:"16px 0"}}>{status}</div>
      <button onClick={handleSelect} style={{padding:"12px 24px", marginRight:12}}>Selecionar ROM .gba</button>
      <button onClick={handlePlay} disabled={!rom} style={{padding:"12px 24px", background: rom?"#1a9e32":"#888", color:"#fff", border:"none", borderRadius:6, cursor: rom?"pointer":"not-allowed"}}>JOGAR</button>
      <p style={{opacity:0.6, marginTop:24}}>Baixou, colocou o .gba de 32MB em /game/ e jogou. Validação 100% local. Nenhuma ROM sai do seu PC.</p>
    </div>
  )
}
