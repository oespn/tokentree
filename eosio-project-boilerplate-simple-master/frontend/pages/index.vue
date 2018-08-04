<template>
  <GmapMap
    :center="{lat:10, lng:10}"
    :zoom="7"
    map-type-id="terrain">
  </GmapMap>
</template>

<style>
.vue-map-container {
  height: 100vh;
  height: 100%;
  width: 100vw;
  width: 100%;
}
</style>
