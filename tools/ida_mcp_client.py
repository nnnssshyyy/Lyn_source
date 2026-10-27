"""Small JSON-RPC client for the locally installed IDA Pro MCP server."""
import argparse
import json
import urllib.request
from pathlib import Path


def call(tool, arguments, timeout=180):
    payload = {"jsonrpc": "2.0", "id": 1, "method": "tools/call",
               "params": {"name": tool, "arguments": arguments}}
    request = urllib.request.Request("http://127.0.0.1:8745/mcp",
                                     json.dumps(payload).encode(),
                                     {"Content-Type": "application/json"})
    with urllib.request.urlopen(request, timeout=timeout) as response:
        result = json.load(response)
    if "error" in result:
        raise RuntimeError(result["error"])
    tool_result = result["result"]
    if tool_result.get("isError"):
        raise RuntimeError(tool_result.get("content", tool_result))
    return tool_result


def data(result):
    if "structuredContent" in result:
        return result["structuredContent"]
    texts = [c["text"] for c in result.get("content", []) if c.get("type") == "text"]
    if len(texts) == 1:
        try:
            return json.loads(texts[0])
        except ValueError:
            pass
    return result


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("tool")
    parser.add_argument("arguments", help="JSON object")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--timeout", type=int, default=180)
    args = parser.parse_args()
    result = data(call(args.tool, json.loads(args.arguments), args.timeout))
    text = json.dumps(result, ensure_ascii=False, indent=2)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    print(text)
