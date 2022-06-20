const modes = [
  'norm', 'custom', 'stream', 'clouds', 'ripple', 'ripple-lines', 'snow',
  'rgb-dots', 'rgb-lines', 'triangular', 'drain', 'matrix', 'scanline',
  'gradient', 'rgb-circle'
];

var keys = null;

const miscSettings = {
  rainbow: false,
  speed: 0,
  brightness: 0,
  direction: 0,
  reprate: 0,
};

const miscIds = ['rainbow', 'speed', 'brightness', 'direction', 'reprate'];

const miscApply = async () => {
  try {
    await sendCmdPromise(`/rainbow/${miscSettings.rainbow.toString()}/`);
    await sendCmdPromise(`/speed/${miscSettings.speed}/`);
    await sendCmdPromise(`/brightness/${miscSettings.brightness}/`);
    //await sendCmdPromise(`/dir/${miscSettings.direction}/`);
    const msg = await sendCmdPromise(`/rate/${miscSettings.reprate}/`);
    document.getElementById('status').style.color = 'green';
    document.getElementById('status').innerText = `misc settings: ${msg}`;
  } catch (err) {
    document.getElementById('status').style.color = 'red';
    document.getElementById('status').innerText = `misc settings: ${err}`;
  }
};

const getColor = () => {
  const r = document.getElementById('color-input__red');
  const g = document.getElementById('color-input__green');
  const b = document.getElementById('color-input__blue');

  return [r, g, b].map(c => parseInt(c.value));
};

const handleKey = k => {
  const mode = document.getElementById('keyboard-mode')['keyboard-mode'].value;

  if (mode === 'color') {
    const [r, g, b] = getColor();
    sendCmd(`/kcolor/${k}/${r}/${g}/${b}/`);
  }
};

const updateMiscSettings = () => {
  const rainbow = document.getElementById('rainbow');
  const speed = document.getElementById('speed');
  const brightness = document.getElementById('brightness');
  const direction = document.getElementById('direction');
  const reprate = document.getElementById('reprate');

  miscSettings.rainbow = rainbow.checked;
  miscSettings.speed = speed.value;
  miscSettings.brightness = brightness.value;
  miscSettings.direction = direction.value;
  miscSettings.reprate = Math.pow(2, parseInt(reprate.value)) * 125;

  miscIds.forEach(id => {
    const val = miscSettings[id];
    document.getElementById(`${id}-value`).innerText = val;
  });
};

const sendCmdPromise = cmd => {
  return new Promise((resolve, reject) => {
    fetch(cmd, { method: 'POST' }).then(res => {
      res.json().then(json => {
        if (json.status === 'ok') {
          resolve(json.msg);
        } else {
          reject(json.msg);
        }
      });
    });
  });
};

const sendCmd = cmd => {
  const status = document.getElementById('status');

  sendCmdPromise(cmd).then(msg => {
    status.style.color = 'green';
    status.innerText = `${cmd}: ${msg}`;
  }).catch(msg => {
    status.style.color = 'red';
    status.innerText = `${cmd}: ${msg}`;
  });
};

const setMode = mode => {
  sendCmd(`/mode/${mode}`);
};

const toHex = num => {
  let hex = num ? parseInt(num).toString(16) : '00';
  if (hex.length < 2) {
    hex = '0' + hex;
  }
  return hex;
};

const rgbToHex = (r, g, b) => {
  return `${toHex(r)}${toHex(g)}${toHex(b)}`;
};

const updateColors = sender => {
  const r = document.getElementById('color-input__red');
  const g = document.getElementById('color-input__green');
  const b = document.getElementById('color-input__blue');
  const hex = document.getElementById('color-input__hex');

  if (sender.classList.contains('btn-palette')) {
    const color = sender.getAttribute('data-color');
    hex.value = color;
    r.value = parseInt(hex.value.substr(0, 2), 16);
    g.value = parseInt(hex.value.substr(2, 2), 16);
    b.value = parseInt(hex.value.substr(4, 2), 16);

  } else if ([r, g, b].includes(sender)) {
    hex.value = rgbToHex(r.value, g.value, b.value);
  } else if (sender === hex) {
    if (!hex.matches(':invalid')) {
      r.value = parseInt(hex.value.substr(0, 2), 16);
      g.value = parseInt(hex.value.substr(2, 2), 16);
      b.value = parseInt(hex.value.substr(4, 2), 16);
    }
  }

  if (!hex.matches(':invalid')) {
    document.getElementById('color-preview').style.backgroundColor
      = `#${hex.value}`;
  }
};

const createKeyboard = (target, keyCallback) => {
  fetch('/keys.json').then(res => {
    if (res.status === 200) {
      res.json().then(json => {
        keys = json;

        for (let keyname in json) {
          const key = json[keyname];
          const btn = document.createElement('button');
          btn.innerText = key.label ?? keyname;
          const [left, top] = key.pos;
          btn.style.top = `${top}px`;
          btn.style.left = `${left}px`;
          btn.style.width = `${key.width ?? 50}px`;
          btn.setAttribute('data-kname', keyname);
          btn.addEventListener('click', e => {
            keyCallback(keyname);
          });
          target.appendChild(btn);
        }
      });
    }
  });
};

const setup = () => {
  modes.forEach(mode => {
    const btn = document.createElement('button');
    btn.classList.add('btn-mode');
    btn.setAttribute('data-mode', mode);
    btn.innerText = `${mode}`;
    btn.addEventListener('click', () => {
      setMode(btn.getAttribute('data-mode'));
    });
    document.getElementById('modes').appendChild(btn);
  });

  document.querySelectorAll('.color-input').forEach(el => {
    el.addEventListener('change', e => {
      updateColors(e.target);
    });
  });

  const palette = document.getElementById('color-palette');
  for (let r = 0; r < 0x100; r += 0x26) {
    for (let g = 0; g < 0x100; g += 0x26) {
      for (let b = 0; b < 0x100; b += 0x26) {
        const btn = document.createElement('button');
        btn.classList.add('btn-palette');
        btn.setAttribute('data-color', rgbToHex(r, g, b));
        btn.style.backgroundColor = '#' + rgbToHex(r, g, b);
        btn.addEventListener('click', e => {
          updateColors(btn);
        });
        palette.appendChild(btn);
      }
    }
  }

  document.getElementById('btn-color').addEventListener('click', e => {
    const [r, g, b] = ['red', 'green', 'blue'].map(c => document.getElementById(`color-input__${c}`).value);
    sendCmd(`/color/${r}/${g}/${b}/`);
  });

  createKeyboard(document.getElementById('keyboard'), handleKey);

  miscIds.forEach(id => {
    const el = document.getElementById(id);
    el.addEventListener('change', updateMiscSettings);
  });

  document.getElementById('misc-apply').addEventListener('click', miscApply);

  updateColors(document.getElementById('color-input__hex'));
  updateMiscSettings();
};
