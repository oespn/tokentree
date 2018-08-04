<template>
  <v-layout column>
    <v-toolbar>
      <v-btn icon nuxt-link to="/">
        <v-icon>keyboard_backspace</v-icon>
      </v-btn>
      <v-toolbar-title>{{ tree.planter }}'s tree</v-toolbar-title>
    </v-toolbar>
    <img src="https://i.ytimg.com/vi/7BPhS5PdXHU/maxresdefault.jpg" height="250px"/>
    {{tree}}
    <v-list two-line>
      <v-list-tile v-for="(menu, index) in menus" :key="index">
        <v-list-tile-action>
          <v-icon>{{ menu.icon }}</v-icon>
        </v-list-tile-action>
        <v-list-tile-content>
          <v-list-tile-title>{{ menu.title }}</v-list-tile-title>
          <v-list-tile-sub-title>{{ menu.subtitle }}</v-list-tile-sub-title>
        </v-list-tile-content>
        <v-list-tile-action v-if="menu.action">
          <v-icon>{{ menu.action.icon }}</v-icon>
        </v-list-tile-action>
      </v-list-tile>
    </v-list>
  </v-layout>
</template>

<script>
import { mapGetters } from 'vuex';
export default {
  computed: {
    ...mapGetters({
      map: 'treesMap'
    }),
    tree () { return this.map[this.$route.params.id] },
    menus () {
      return [
        { icon: 'person', title: this.tree.planter, subtitle: 'Planter' },
        { icon: 'event', title: this.tree.plantedAt, subtitle: 'Plantation date' },
        { icon: 'location_on', title: this.tree.address.street, subtitle: this.tree.address.city, action: {
          icon: 'near_me'
        } },
        { icon: 'verified_user', title: `${this.tree.verifications.length} verification(s)`, subtitle: 'Number of verifications', action: {
          icon: 'chevron_right'
        } }
      ]
    }
  }
}
</script>
