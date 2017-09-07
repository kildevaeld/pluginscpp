const fs = require('fs');

const argv = process.argv.slice(2);
if (argv.length != 2) {
  console.error('usage: generate <input> <output>');
  process.exit(2);
}

const input = fs.readFileSync(argv[0], 'utf8');

try {
  input = JSON.stringify(JSON.parse(input), null, "")
} catch (e) {}

var out = []
for (let i = 0; i < input.length; i++) {
  out.push('0x' + input.charCodeAt(i).toString(16));
}
out.push('0x0');
fs.writeFileSync(argv[1], generate(out));


function generate(slice) {
  return `const unsigned char plugin_metadata_json[] = { ${slice.join(',')} };\nunsigned int plugin_metadata_json_len = ${slice.length+1};`
}