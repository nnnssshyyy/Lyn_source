"""Export the read-only Raze stream-proof evidence from an existing IDA MCP IDB."""
import argparse
import hashlib
import json
import sys
from datetime import datetime, timezone
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ida_mcp_client import call, data

ROOT = Path(__file__).resolve().parents[2]
EXPECTED = "dfa8632e85c89c318577ef619f20bae2f4db5136042d680ee4dae4109234691a"
FUNCTIONS = {
    "endscene": "0x1016e6d0", "install-endscene": "0x1016e6b0",
    "paint": "0x1016cc80", "reset": "0x1016ea20",
    "install-reset": "0x1016ea00", "dx9-renderer": "0x1006cc30",
    "dx9-state": "0x1006d0d0", "init-once": "0x101827d8",
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--database", default="raze-antiobs-review")
    parser.add_argument("--binary", type=Path, default=Path.home() / "Desktop/raze/RZL4D2_[unknowncheats.me]_.dll")
    parser.add_argument("--output", type=Path, default=ROOT / "docs/re/anti-obs/recheck-20261021")
    args = parser.parse_args()
    if hashlib.sha256(args.binary.read_bytes()).hexdigest() != EXPECTED:
        raise RuntimeError("The reference DLL differs from the analyzed Raze release")
    args.output.mkdir(parents=True, exist_ok=True)
    exported = {}

    def export(name, tool, **arguments):
        value = data(call(tool, {"database": args.database, **arguments}))
        if isinstance(value, dict) and value.get("error"):
            raise RuntimeError(value["error"])
        path = args.output / (name + ".json")
        path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        exported[path.name] = hashlib.sha256(path.read_bytes()).hexdigest()
        return value

    survey = export("survey", "survey_binary", detail_level="minimal")
    if survey["metadata"]["sha256"].lower() != EXPECTED:
        raise RuntimeError("IDA input hash differs from the original DLL")
    export("strings", "find_regex", pattern="^bStreamProof$|Hides visual elements|Oculta elementos visuales|^Streamer mode$", limit=30)
    xrefs = export("flag-xrefs", "xref_query", queries={"addr": "0x103f5396", "direction": "to", "include_fn": True, "count": 100})
    export("callback-xrefs", "xref_query", queries=[{"addr": address, "direction": "to", "include_fn": True, "count": 100} for address in ["0x1016e6d0", "0x103f746c", "0x103f73b0"]])
    export("imports", "imports_query", queries=[{"filter": pattern, "count": 100} for pattern in ["*Window*", "*Affinity*", "*InitOnce*", "*ProcAddress*", "*ModuleFileName*", "*VirtualQuery*"]])
    export("caller-substring-bytes", "get_bytes", regions={"addr": "0x10261e00", "size": 16})

    total_instructions = 0
    for name, address in FUNCTIONS.items():
        export(name, "decompile", addr=address, max_lines=5000)
        assembly = export(name + "-asm", "disasm", addr=address, max_instructions=5000, include_total=True)
        if not assembly.get("cursor", {}).get("done"):
            raise RuntimeError("Incomplete instruction export for " + name)
        total_instructions += assembly.get("instruction_count", 0)

    export("flag-branch-sites", "insn_query", queries=[{
        "start": item["from"], "end": hex(int(item["from"], 16) + 40),
        "count": 24, "include_disasm": True,
    } for item in xrefs["result"][0]["data"]])
    export("config-and-ui", "insn_query", queries=[{
        "start": start, "end": end, "count": 100, "include_disasm": True,
    } for start, end in [("0x1000959e", "0x100095e5"), ("0x100e57ec", "0x100e5858"), ("0x1010bf31", "0x1010bf87")]])
    export("material-guard", "insn_query", queries={"start": "0x1014d5b7", "end": "0x1014d5c0", "count": 10, "include_disasm": True})
    export("glow-guard", "insn_query", queries={"start": "0x1014e630", "end": "0x1014e67e", "count": 60, "include_disasm": True})
    manifest = {
        "created_at_utc": datetime.now(timezone.utc).isoformat(),
        "database": args.database, "original_binary": str(args.binary),
        "sha256": EXPECTED, "image_base": "0x10000000",
        "complete_function_exports": len(FUNCTIONS), "instructions_exported": total_instructions,
        "streamproof_flag_references": xrefs["result"][0]["total"],
        "binary_executed": False, "runtime_dll_changed": False,
        "capture_or_fps_benchmarked": False, "files": exported,
    }
    (args.output / "manifest.json").write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({key: value for key, value in manifest.items() if key != "files"}, ensure_ascii=False))


if __name__ == "__main__":
    main()
