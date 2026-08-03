import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'

// vite.config.js
export default {
  server: {
    proxy: {
      '/Interpreter': 'http://localhost:5112'
    }
  }
}
