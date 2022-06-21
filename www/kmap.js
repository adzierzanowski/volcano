KMAP_KEYNAMES = [
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
  'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
  '5', '6', '7', '8', '9', '0', 'ENTER', 'ESC', 'BACKSPACE', 'TAB',
  'SPACE', 'MINUS', 'PLUS', 'LBRACKET', 'RBRACKET', 'BACKSLASH', 'COLON', 'APOSTROPHE',
  'TILDE', 'LT', 'GT', 'SLASH', 'CAPS', 'F1', 'F2', 'F3', 'F4', 'F5', 'F6',
  'F7', 'F8', 'F9', 'F10', 'F11', 'F12', 'PS', 'SL', 'PB', 'INS', 'HM', 'PU',
  'DEL', 'END', 'PD', 'RIGHT', 'LEFT', 'DOWN', 'UP', 'LSHIFT', 'RSHIFT', 'LALT',
  'RALT', 'LCTRL', 'RCTRL', 'META', 'MENU'
];

const mapping = {};

const updateRemappedKeyboard = () => {
  const remapped = document.getElementById('keyboard-remapped');

  remapped.querySelectorAll('button').forEach(btn => {
    btn.classList.remove('remapped');
    const kname = btn.getAttribute('data-kname');
    btn.innerText = keys[kname]?.label ?? kname;
  });


  for (let key in mapping) {
    const remappedKey = remapped.querySelector(`button[data-kname=${key}]`);
    remappedKey.innerText = keys?.[mapping[key]]?.label ?? mapping[key];
    remappedKey.classList.add('remapped');
  }
};

const handleKmapKey = kname => {
  console.log(kname, keys[kname]);
  const remap = prompt(`Remap ${kname} to`);
  if (KMAP_KEYNAMES.includes(remap) || !isNaN(parseInt(remap))) {
    mapping[kname] = remap;
  } else {
    alert('Bad key name.');
  }
  updateRemappedKeyboard();
};

const removeMapping = kname => {
  delete mapping[kname];
  updateRemappedKeyboard();
};

createKeyboard(document.getElementById('keyboard'), handleKmapKey);
createKeyboard(document.getElementById('keyboard-remapped'), removeMapping);

const findKeyByCode = code => {
  for (let key in keys) {
    const k = keys[key];
    if (k.code === code) {
      return k;
    }
  }
  return null;
};

const generateKmap = () => {
  let kmap = [];
  let mkmap = [];

  for (let kname of KMAP_KEYNAMES) {
    if (kname in mapping) {
      const key = keys[mapping[kname]];
      if (key) {
        kmap.push(key.code);
        mkmap.push(key?.modal ? 1 : 2);
      } else {
        kmap.push(parseInt(mapping[kname]));
        mkmap.push(2);
      }
    } else {
      const key = keys[kname];
      kmap.push(parseInt(key.code));
      mkmap.push(key?.modal ? 1 : 2);
    }
  }

  return [...kmap, ...mkmap];
};


document.getElementById('btn-help').addEventListener('click', e => {
  document.getElementById('help').classList.toggle('open');
});

document.getElementById('btn-save').addEventListener('click', e => {
  const kmap = generateKmap();
  const out = new Uint8Array(kmap);
  const blob = new Blob([out], { type: 'application/octet-stream' });
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.setAttribute('href', url);
  link.setAttribute('download', 'kmap.dat');
  link.click();
  link.remove();
});

document.getElementById('btn-upload').addEventListener('click', e => {
  const kmap = generateKmap();
  console.log(kmap);
  fetch('/kmap', {
    method: 'POST',
    body: JSON.stringify(kmap),
    headers: { 'Content-Type': 'application/json' }
  }).then(res => console.log(res));
});

document.getElementById('keynames').innerHTML = KMAP_KEYNAMES.map(kname => `<div class="keyname">${kname}</div>`).join(' ');
