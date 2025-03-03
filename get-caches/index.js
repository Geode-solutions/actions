const core = require("@actions/core")
const cache = require("@actions/cache")
const fs = require("fs")

const main = async () => {
  try {
    const system = core.getInput("system")
    console.log("system", system)
    const keys = core.getInput("keys").split(";")
    console.log("keys", keys)
    if (keys.length == 0) {
      return 0
    }
    for (let key of keys) {
      if (!key.length) {
        return
      }
      console.log("key", key)
      const system_key = system + "-" + key
      console.log("Looking for cache:", system_key)
      const cacheKey = await cache.restoreCache(["build/install"], system_key)
      console.log("cacheKey", cacheKey)
      if (!cacheKey) {
        core.setFailed("Failed to retrieve cache for " + system_key)
        return
      }
    }
    fs.renameSync("build/install", "prefix")
  } catch (error) {
    core.setFailed(error.message)
  }
}

// Call the main function to run the action
main()
