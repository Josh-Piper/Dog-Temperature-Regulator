import { Inter } from 'next/font/google';
import axios from 'axios';
import Particle from 'particle-api-js';
import React, { useState, useEffect } from 'react';

const inter = Inter({ subsets: ['latin'] });

// The funky naming identifiers is to be consistent with the identifiers in the Argon code.
// TODO: Update identifiers to camelCase.
export default function Home() {
  const particle = new Particle();

  // Using an access token is bad practise since it is hard coded into the user interface.
  // Instead, we *should* prompt a user to login. See https://docs.particle.io/reference/cloud-apis/access-tokens/
  const accessToken = 'accessTokenFromParticleArgon';
  const deviceId = 'deviceIdFromParticleArgon';
  
  const particleDomain = 'https://api.particle.io/v1/devices';

  // DANGEROUS_DOG_TOO_COLD_TEMPERATURE
  const updateDangerousDogTooColdTemperature = async () => {
    const result = await particle.callFunction({
      deviceId,
      name: 'updateDangerousDogTooColdTemperature',
      argument: `${NEW_DANGEROUS_DOG_TOO_COLD_TEMPERATURE}`,
      auth: accessToken,
    });

    if (result.statusCode === 200) {
      setDANGEROUS_DOG_TOO_COLD_TEMPERATURE(
        NEW_DANGEROUS_DOG_TOO_COLD_TEMPERATURE
      );
    } else {
      setDANGEROUS_DOG_TOO_COLD_TEMPERATURE(-999);
    }
  };

  const [
    DANGEROUS_DOG_TOO_COLD_TEMPERATURE,
    setDANGEROUS_DOG_TOO_COLD_TEMPERATURE,
  ] = useState(1);
  const [
    NEW_DANGEROUS_DOG_TOO_COLD_TEMPERATURE,
    setNEW_DANGEROUS_DOG_TOO_COLD_TEMPERATURE,
  ] = useState(-1);

  const updateNewTooColdTemperature = (e) => {
    setNEW_DANGEROUS_DOG_TOO_COLD_TEMPERATURE(e.target.value);
  };

  // Load in the actual temperature values, may be the default since by the Particle Argon hard coded values.
  useEffect(() => {
    axios
      .get(
        `${particleDomain}/${deviceId}/DANGEROUS_DOG_TOO_COLD_TEMPERATURE?access_token=${accessToken}`
      )
      .then((res) => {
        setDANGEROUS_DOG_TOO_COLD_TEMPERATURE(res.data.result);
      });
  });

  // DANGEROUS_DOG_TOO_HOT_TEMPERATURE
  const updateDangerousDogTooHotTemperature = async () => {
    const result = await particle.callFunction({
      deviceId,
      name: 'updateDangerousDogTooHotTemperature',
      argument: `${NEW_DANGEROUS_DOG_TOO_HOT_TEMPERATURE}`,
      auth: accessToken,
    });

    if (result.statusCode === 200) {
      setDANGEROUS_DOG_TOO_HOT_TEMPERATURE(
        NEW_DANGEROUS_DOG_TOO_HOT_TEMPERATURE
      );
    } else {
      setDANGEROUS_DOG_TOO_HOT_TEMPERATURE(-999);
    }
  };

  const [
    DANGEROUS_DOG_TOO_HOT_TEMPERATURE,
    setDANGEROUS_DOG_TOO_HOT_TEMPERATURE,
  ] = useState(1);
  const [
    NEW_DANGEROUS_DOG_TOO_HOT_TEMPERATURE,
    setNEW_DANGEROUS_DOG_TOO_HOT_TEMPERATURE,
  ] = useState(-1);

  const updateNewTooHotTemperature = (e) => {
    setNEW_DANGEROUS_DOG_TOO_HOT_TEMPERATURE(e.target.value);
  };

  // Load in the actual temperature values, may be the default since by the Particle Argon hard coded values.
  useEffect(() => {
    axios
      .get(
        `${particleDomain}/${deviceId}/DANGEROUS_DOG_TOO_HOT_TEMPERATURE?access_token=${accessToken}`
      )
      .then((res) => {
        setDANGEROUS_DOG_TOO_HOT_TEMPERATURE(res.data.result);
      });
  });

  return (
    <main className="p-4">
      <h1 className="mb-4 text-4xl font-extrabold leading-none tracking-tight text-gray-900 md:text-5xl lg:text-6xl dark:text-white">
        Pet &nbsp;
        <span className="text-blue-600 dark:text-blue-500">Temperature</span> &nbsp;
        Regulator
      </h1>
      <p className="text-lg font-normal text-gray-500 lg:text-xl dark:text-gray-400">
        Set the minimum and maximum optimal temperature range for your pet!
      </p>
      <a
        href="https://github.com/Josh-Piper/Dog-Temperature-Regulator"
        className="text-lg font-normal text-blue-800 underline lg:text-xl dark:text-blue-800"
      >
        Find out more about this project
      </a>

      <br />
      <br />

      <div className="z-10 w-full max-w-5xl items-center font-mono text-sm lg:flex">
        <p>
          Current optimal low temperature (celsuis):{' '}
          <span>{DANGEROUS_DOG_TOO_COLD_TEMPERATURE}</span>
        </p>
      </div>

      <div className="z-10 w-full max-w-5xl items-center justify-between font-mono text-sm lg:flex">
        <input
          className="shadow appearance-none border rounded py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
          id="username"
          type="text"
          onChange={updateNewTooColdTemperature}
          value={NEW_DANGEROUS_DOG_TOO_COLD_TEMPERATURE}
        />
        <button
          className="bg-white hover:bg-gray-100 text-gray-800 font-semibold py-2 px-4 border border-gray-400 rounded shadow"
          onClick={() => updateDangerousDogTooColdTemperature()}
        >
          Update New Low Temperature
        </button>
      </div>

      <br />

      <div className="z-10 w-full max-w-5xl items-center font-mono text-sm lg:flex">
        <p>
          Current optimal top temperature (celsuis):{' '}
          <span>{DANGEROUS_DOG_TOO_HOT_TEMPERATURE}</span>
        </p>
      </div>

      <div className="z-10 w-full max-w-5xl items-center justify-between font-mono text-sm lg:flex">
        <input
          className="shadow appearance-none border rounded py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
          id="username"
          type="text"
          onChange={updateNewTooHotTemperature}
          value={NEW_DANGEROUS_DOG_TOO_HOT_TEMPERATURE}
        />
        <button
          className="bg-white hover:bg-gray-100 text-gray-800 font-semibold py-2 px-4 border border-gray-400 rounded shadow"
          onClick={() => updateDangerousDogTooHotTemperature()}
        >
          Update New Hot Temperature
        </button>
      </div>
    </main>
  );
}
